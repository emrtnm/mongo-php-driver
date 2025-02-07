/*
 * Copyright 2014-present MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "bson/bson.h"

#include <php.h>
#include <Zend/zend_interfaces.h>

#include "php_array_api.h"

#include "php_phongo.h"
#include "phongo_bson_encode.h"
#include "phongo_error.h"

zend_class_entry* php_phongo_command_ce;

/* Initialize the "maxAwaitTimeMS" option. Returns true on success; otherwise,
 * false is returned and an exception is thrown.
 *
 * The "maxAwaitTimeMS" option is assigned to the cursor after query execution
 * via mongoc_cursor_set_max_await_time_ms(). */
static bool php_phongo_command_init_max_await_time_ms(php_phongo_command_t* intern, zval* options) /* {{{ */
{
	int64_t max_await_time_ms;

	if (!php_array_existsc(options, "maxAwaitTimeMS")) {
		return true;
	}

	max_await_time_ms = php_array_fetchc_long(options, "maxAwaitTimeMS");

	if (max_await_time_ms < 0) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"maxAwaitTimeMS\" option to be >= 0, %" PRId64 " given", max_await_time_ms);
		return false;
	}

	if (max_await_time_ms > UINT32_MAX) {
		phongo_throw_exception(PHONGO_ERROR_INVALID_ARGUMENT, "Expected \"maxAwaitTimeMS\" option to be <= %" PRIu32 ", %" PRId64 " given", UINT32_MAX, max_await_time_ms);
		return false;
	}

	intern->max_await_time_ms = (uint32_t) max_await_time_ms;

	return true;
} /* }}} */

/* Initializes the php_phongo_command_init from options argument. This
 * function will fall back to a modifier in the absence of a top-level option
 * (where applicable). */
static bool php_phongo_command_init(php_phongo_command_t* intern, zval* filter, zval* options) /* {{{ */
{
	bson_iter_t iter;
	bson_iter_t sub_iter;

	intern->bson              = bson_new();
	intern->batch_size        = 0;
	intern->max_await_time_ms = 0;

	php_phongo_zval_to_bson(filter, PHONGO_BSON_NONE, intern->bson, NULL);

	/* Note: if any exceptions are thrown, we can simply return as PHP will
	 * invoke php_phongo_query_free_object to destruct the object. */
	if (EG(exception)) {
		return false;
	}

	if (bson_iter_init(&iter, intern->bson) && bson_iter_find_descendant(&iter, "cursor.batchSize", &sub_iter) && BSON_ITER_HOLDS_INT(&sub_iter)) {
		int64_t batch_size = bson_iter_as_int64(&sub_iter);

		if (batch_size >= 0 && batch_size <= UINT32_MAX) {
			intern->batch_size = (uint32_t) batch_size;
		}
	}

	if (!options) {
		return true;
	}

	if (!php_phongo_command_init_max_await_time_ms(intern, options)) {
		return false;
	}

	return true;
} /* }}} */

/* {{{ proto void MongoDB\Driver\Command::__construct(array|object $document[, array $options = array()])
   Constructs a new Command */
static PHP_METHOD(Command, __construct)
{
	zend_error_handling   error_handling;
	php_phongo_command_t* intern;
	zval*                 document;
	zval*                 options = NULL;

	intern = Z_COMMAND_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "A|a!", &document, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	php_phongo_command_init(intern, document, options);
} /* }}} */

/* {{{ MongoDB\Driver\Command function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_Command___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, document)
	ZEND_ARG_ARRAY_INFO(0, options, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Command_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_command_me[] = {
	/* clang-format off */
	PHP_ME(Command, __construct, ai_Command___construct, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_Command_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\Command object handlers */
static zend_object_handlers php_phongo_handler_command;

static void php_phongo_command_free_object(zend_object* object) /* {{{ */
{
	php_phongo_command_t* intern = Z_OBJ_COMMAND(object);

	zend_object_std_dtor(&intern->std);

	if (intern->bson) {
		bson_clear(&intern->bson);
	}
} /* }}} */

static zend_object* php_phongo_command_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_command_t* intern = zend_object_alloc(sizeof(php_phongo_command_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	intern->std.handlers = &php_phongo_handler_command;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_command_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	php_phongo_command_t* intern;
	zval                  retval = ZVAL_STATIC_INIT;

	*is_temp = 1;
	intern   = Z_OBJ_COMMAND(PHONGO_COMPAT_GET_OBJ(object));

	array_init_size(&retval, 1);

	if (intern->bson) {
		zval zv;

		if (!php_phongo_bson_to_zval(bson_get_data(intern->bson), intern->bson->len, &zv)) {
			zval_ptr_dtor(&zv);
			goto done;
		}

		ADD_ASSOC_ZVAL_EX(&retval, "command", &zv);
	} else {
		ADD_ASSOC_NULL_EX(&retval, "command");
	}

done:
	return Z_ARRVAL(retval);

} /* }}} */
/* }}} */

void php_phongo_command_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Command", php_phongo_command_me);
	php_phongo_command_ce                = zend_register_internal_class(&ce);
	php_phongo_command_ce->create_object = php_phongo_command_create_object;
	PHONGO_CE_FINAL(php_phongo_command_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_command_ce);

	memcpy(&php_phongo_handler_command, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_command.get_debug_info = php_phongo_command_get_debug_info;
	php_phongo_handler_command.free_obj       = php_phongo_command_free_object;
	php_phongo_handler_command.offset         = XtOffsetOf(php_phongo_command_t, std);
} /* }}} */
