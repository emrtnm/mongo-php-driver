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

#include <php.h>

#include "php_phongo.h"

zend_class_entry* php_phongo_connectionexception_ce;

/* {{{ MongoDB\Driver\Exception\ConnectionException function entries */
static zend_function_entry php_phongo_connectionexception_me[] = {
	PHP_FE_END
};
/* }}} */

void php_phongo_connectionexception_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver\\Exception", "ConnectionException", php_phongo_connectionexception_me);
	php_phongo_connectionexception_ce = zend_register_internal_class_ex(&ce, php_phongo_runtimeexception_ce);
} /* }}} */
