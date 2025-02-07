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
#include "mongoc/mongoc.h"

#include <php.h>
#include <Zend/zend_interfaces.h>

#include "php_phongo.h"
#include "phongo_bson_encode.h"
#include "phongo_client.h"
#include "phongo_error.h"
#include "phongo_execute.h"
#include "phongo_util.h"

#include "MongoDB/Server.h"
#include "MongoDB/ServerDescription.h"

zend_class_entry* php_phongo_server_ce;

/* {{{ proto MongoDB\Driver\Cursor MongoDB\Driver\Server::executeCommand(string $db, MongoDB\Driver\Command $command[, array $options = null]))
   Executes a Command on this Server */
static PHP_METHOD(Server, executeCommand)
{
	zend_error_handling  error_handling;
	php_phongo_server_t* intern;
	char*                db;
	size_t               db_len;
	zval*                command;
	zval*                options      = NULL;
	bool                 free_options = false;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sO|z!", &db, &db_len, &command, php_phongo_command_ce, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	options = php_phongo_prep_legacy_option(options, "readPreference", &free_options);

	/* If the Server was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, Z_MANAGER_OBJ_P(&intern->manager));

	phongo_execute_command(&intern->manager, PHONGO_COMMAND_RAW, db, command, options, intern->server_id, return_value);

	if (free_options) {
		php_phongo_prep_legacy_option_free(options);
	}
} /* }}} */

/* {{{ proto MongoDB\Driver\Cursor MongoDB\Driver\Server::executeReadCommand(string $db, MongoDB\Driver\Command $command[, array $options = null]))
   Executes a ReadCommand on this Server */
static PHP_METHOD(Server, executeReadCommand)
{
	zend_error_handling  error_handling;
	php_phongo_server_t* intern;
	char*                db;
	size_t               db_len;
	zval*                command;
	zval*                options = NULL;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sO|a!", &db, &db_len, &command, php_phongo_command_ce, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	/* If the Server was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, Z_MANAGER_OBJ_P(&intern->manager));

	phongo_execute_command(&intern->manager, PHONGO_COMMAND_READ, db, command, options, intern->server_id, return_value);
} /* }}} */

/* {{{ proto MongoDB\Driver\Cursor MongoDB\Driver\Server::executeWriteCommand(string $db, MongoDB\Driver\Command $command[, array $options = null]))
   Executes a WriteCommand on this Server */
static PHP_METHOD(Server, executeWriteCommand)
{
	zend_error_handling  error_handling;
	php_phongo_server_t* intern;
	char*                db;
	size_t               db_len;
	zval*                command;
	zval*                options = NULL;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sO|a!", &db, &db_len, &command, php_phongo_command_ce, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	/* If the Server was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated. and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, Z_MANAGER_OBJ_P(&intern->manager));

	phongo_execute_command(&intern->manager, PHONGO_COMMAND_WRITE, db, command, options, intern->server_id, return_value);
} /* }}} */

/* {{{ proto MongoDB\Driver\Cursor MongoDB\Driver\Server::executeReadWriteCommand(string $db, MongoDB\Driver\Command $command[, array $options = null]))
   Executes a ReadWriteCommand on this Server */
static PHP_METHOD(Server, executeReadWriteCommand)
{
	zend_error_handling  error_handling;
	php_phongo_server_t* intern;
	char*                db;
	size_t               db_len;
	zval*                command;
	zval*                options = NULL;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sO|a!", &db, &db_len, &command, php_phongo_command_ce, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	/* If the Server was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, Z_MANAGER_OBJ_P(&intern->manager));

	phongo_execute_command(&intern->manager, PHONGO_COMMAND_READ_WRITE, db, command, options, intern->server_id, return_value);
} /* }}} */

/* {{{ proto MongoDB\Driver\Cursor MongoDB\Driver\Server::executeQuery(string $namespace, MongoDB\Driver\Query $query[, array $options = null]))
   Executes a Query on this Server */
static PHP_METHOD(Server, executeQuery)
{
	zend_error_handling  error_handling;
	php_phongo_server_t* intern;
	char* namespace;
	size_t namespace_len;
	zval*  query;
	zval*  options      = NULL;
	bool   free_options = false;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sO|z!", &namespace, &namespace_len, &query, php_phongo_query_ce, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	options = php_phongo_prep_legacy_option(options, "readPreference", &free_options);

	/* If the Server was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, Z_MANAGER_OBJ_P(&intern->manager));

	phongo_execute_query(&intern->manager, namespace, query, options, intern->server_id, return_value);

	if (free_options) {
		php_phongo_prep_legacy_option_free(options);
	}
} /* }}} */

/* {{{ proto MongoDB\Driver\WriteResult MongoDB\Driver\Server::executeBulkWrite(string $namespace, MongoDB\Driver\BulkWrite $zbulk[, array $options = null])
   Executes a BulkWrite (i.e. any number of insert, update, and delete ops) on
   this Server */
static PHP_METHOD(Server, executeBulkWrite)
{
	zend_error_handling  error_handling;
	php_phongo_server_t* intern;
	char* namespace;
	size_t                  namespace_len;
	zval*                   zbulk;
	php_phongo_bulkwrite_t* bulk;
	zval*                   options      = NULL;
	bool                    free_options = false;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sO|z!", &namespace, &namespace_len, &zbulk, php_phongo_bulkwrite_ce, &options, php_phongo_writeconcern_ce) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	bulk = Z_BULKWRITE_OBJ_P(zbulk);

	options = php_phongo_prep_legacy_option(options, "writeConcern", &free_options);

	/* If the Server was created in a different process, reset the client so
	 * that its session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern, Z_MANAGER_OBJ_P(&intern->manager));

	phongo_execute_bulk_write(&intern->manager, namespace, bulk, options, intern->server_id, return_value);

	if (free_options) {
		php_phongo_prep_legacy_option_free(options);
	}
} /* }}} */

/* {{{ proto string MongoDB\Driver\Server::getHost()
   Returns the hostname for this Server */
static PHP_METHOD(Server, getHost)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		RETVAL_STRING(mongoc_server_description_host(sd)->host);
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto array MongoDB\Driver\Server::getTags()
   Returns the currently configured tags for this Server */
static PHP_METHOD(Server, getTags)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		const bson_t* hello_response = mongoc_server_description_hello_response(sd);
		bson_iter_t   iter;

		if (bson_iter_init_find(&iter, hello_response, "tags") && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
			const uint8_t*        bytes;
			uint32_t              len;
			php_phongo_bson_state state;

			PHONGO_BSON_INIT_DEBUG_STATE(state);
			bson_iter_document(&iter, &len, &bytes);

			if (!php_phongo_bson_to_zval_ex(bytes, len, &state)) {
				/* Exception should already have been thrown */
				zval_ptr_dtor(&state.zchild);
				mongoc_server_description_destroy(sd);
				return;
			}

			mongoc_server_description_destroy(sd);

			RETURN_ZVAL(&state.zchild, 0, 1);
		}

		array_init(return_value);
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto array MongoDB\Driver\Server::getInfo()
   Returns the last hello response for this Server or, in the case of a load
   balancer, the initial handshake response. */
static PHP_METHOD(Server, getInfo)
{
	php_phongo_server_t*         intern;
	mongoc_client_t*             client;
	mongoc_server_description_t* sd;
	mongoc_server_description_t* handshake_sd = NULL;
	const bson_t*                hello_response;
	php_phongo_bson_state        state;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_SERVER_OBJ_P(getThis());
	client = Z_MANAGER_OBJ_P(&intern->manager)->client;

	if (!(sd = mongoc_client_get_server_description(client, intern->server_id))) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
		return;
	}

	hello_response = mongoc_server_description_hello_response(sd);

	/* If the server description is a load balancer, its hello_response will be
	 * empty. Instead, report the hello_response from the handshake description
	 * (i.e. backing server). */
	if (!strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_LOAD_BALANCER].name)) {
		bson_error_t error = { 0 };

		if (!(handshake_sd = mongoc_client_get_handshake_description(client, intern->server_id, NULL, &error))) {
			phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get handshake server description: %s", error.message);
			goto cleanup;
		}

		hello_response = mongoc_server_description_hello_response(handshake_sd);
	}

	PHONGO_BSON_INIT_DEBUG_STATE(state);

	if (!php_phongo_bson_to_zval_ex(bson_get_data(hello_response), hello_response->len, &state)) {
		/* Exception should already have been thrown */
		zval_ptr_dtor(&state.zchild);
		goto cleanup;
	}

	RETVAL_ZVAL(&state.zchild, 0, 1);

cleanup:
	if (handshake_sd) {
		mongoc_server_description_destroy(handshake_sd);
	}

	mongoc_server_description_destroy(sd);
} /* }}} */

/* {{{ proto integer|null MongoDB\Driver\Server::getLatency()
   Returns the measured latency (i.e. round trip time in milliseconds) for
   this Server, or null if unset. */
static PHP_METHOD(Server, getLatency)
{
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	PHONGO_PARSE_PARAMETERS_NONE();

	intern = Z_SERVER_OBJ_P(getThis());

	if (!(sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
		return;
	}

	/* TODO: Use MONGOC_RTT_UNSET once it is added to libmongoc's public API (CDRIVER-4176) */
	if (mongoc_server_description_round_trip_time(sd) == -1) {
		RETVAL_NULL();
	} else {
		RETVAL_LONG((zend_long) mongoc_server_description_round_trip_time(sd));
	}

	mongoc_server_description_destroy(sd);
} /* }}} */

/* {{{ proto integer MongoDB\Driver\Server::getPort()
   Returns the port for this Server */
static PHP_METHOD(Server, getPort)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		RETVAL_LONG(mongoc_server_description_host(sd)->port);
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto MongoDB\Driver\ServerDescription MongoDB\Driver\Server::getServerDescription()
   Returns the server description for this Server */
static PHP_METHOD(Server, getServerDescription)
{
	mongoc_server_description_t* server_description;
	php_phongo_server_t*         intern = Z_SERVER_OBJ_P(getThis());

	PHONGO_PARSE_PARAMETERS_NONE();

	server_description = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id);

	/* Avoid making another copy in phongo_serverdescription_init */
	phongo_serverdescription_init_ex(return_value, server_description, false);
} /* }}} */

/* {{{ proto integer MongoDB\Driver\Server::getType()
   Returns the node type of this Server */
static PHP_METHOD(Server, getType)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		RETVAL_LONG(php_phongo_server_description_type(sd));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto boolean MongoDB\Driver\Server::isPrimary()
   Returns whether this Server is a primary member of a replica set */
static PHP_METHOD(Server, isPrimary)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		RETVAL_BOOL(!strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_RS_PRIMARY].name));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto boolean MongoDB\Driver\Server::isSecondary()
   Returns whether this Server is a secondary member of a replica set */
static PHP_METHOD(Server, isSecondary)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		RETVAL_BOOL(!strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_RS_SECONDARY].name));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto boolean MongoDB\Driver\Server::isArbiter()
   Returns whether this Server is an arbiter member of a replica set */
static PHP_METHOD(Server, isArbiter)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		RETVAL_BOOL(!strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_RS_ARBITER].name));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto boolean MongoDB\Driver\Server::isHidden()
   Returns whether this Server is a hidden member of a replica set */
static PHP_METHOD(Server, isHidden)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		bson_iter_t iter;

		RETVAL_BOOL(bson_iter_init_find_case(&iter, mongoc_server_description_hello_response(sd), "hidden") && bson_iter_as_bool(&iter));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto boolean MongoDB\Driver\Server::isPassive()
   Returns whether this Server is a passive member of a replica set */
static PHP_METHOD(Server, isPassive)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern->manager)->client, intern->server_id))) {
		bson_iter_t iter;

		RETVAL_BOOL(bson_iter_init_find_case(&iter, mongoc_server_description_hello_response(sd), "passive") && bson_iter_as_bool(&iter));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ MongoDB\Driver\Server function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, db)
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Driver\\Command, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeRWCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, db)
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Driver\\Command, 0)
	ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeQuery, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_OBJ_INFO(0, zquery, MongoDB\\Driver\\Query, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeBulkWrite, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_OBJ_INFO(0, zbulk, MongoDB\\Driver\\BulkWrite, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Server_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_server_me[] = {
	/* clang-format off */
	PHP_ME(Server, executeCommand, ai_Server_executeCommand, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, executeReadCommand, ai_Server_executeRWCommand, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, executeWriteCommand, ai_Server_executeRWCommand, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, executeReadWriteCommand, ai_Server_executeRWCommand, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, executeQuery, ai_Server_executeQuery, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, executeBulkWrite, ai_Server_executeBulkWrite, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, getHost, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, getTags, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, getInfo, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, getLatency, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, getPort, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, getServerDescription, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, getType, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, isPrimary, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, isSecondary, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, isArbiter, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, isHidden, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, isPassive, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_Server_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\Server object handlers */
static zend_object_handlers php_phongo_handler_server;

static int php_phongo_server_compare_objects(zval* o1, zval* o2) /* {{{ */
{
	php_phongo_server_t*         intern1;
	php_phongo_server_t*         intern2;
	mongoc_server_description_t *sd1, *sd2;
	int                          retval = 0;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_SERVER_OBJ_P(o1);
	intern2 = Z_SERVER_OBJ_P(o2);

	sd1 = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern1->manager)->client, intern1->server_id);
	sd2 = mongoc_client_get_server_description(Z_MANAGER_OBJ_P(&intern2->manager)->client, intern2->server_id);

	if (sd1 && sd2) {
		retval = strcasecmp(mongoc_server_description_host(sd1)->host_and_port, mongoc_server_description_host(sd2)->host_and_port);
	} else {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description(s)");
	}

	if (sd1) {
		mongoc_server_description_destroy(sd1);
	}

	if (sd2) {
		mongoc_server_description_destroy(sd2);
	}

	return retval;
} /* }}} */

static void php_phongo_server_free_object(zend_object* object) /* {{{ */
{
	php_phongo_server_t* intern = Z_OBJ_SERVER(object);

	zend_object_std_dtor(&intern->std);

	if (!Z_ISUNDEF(intern->manager)) {
		zval_ptr_dtor(&intern->manager);
	}
} /* }}} */

static zend_object* php_phongo_server_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_server_t* intern = zend_object_alloc(sizeof(php_phongo_server_t), class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	PHONGO_SET_CREATED_BY_PID(intern);

	intern->std.handlers = &php_phongo_handler_server;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_server_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	php_phongo_server_t*         intern = NULL;
	zval                         retval = ZVAL_STATIC_INIT;
	mongoc_client_t*             client;
	mongoc_server_description_t* sd;

	*is_temp = 1;
	intern   = Z_OBJ_SERVER(PHONGO_COMPAT_GET_OBJ(object));
	client   = Z_MANAGER_OBJ_P(&intern->manager)->client;

	if (!(sd = mongoc_client_get_server_description(client, intern->server_id))) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
		return NULL;
	}

	php_phongo_server_to_zval(&retval, client, sd);
	mongoc_server_description_destroy(sd);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_server_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Server", php_phongo_server_me);
	php_phongo_server_ce                = zend_register_internal_class(&ce);
	php_phongo_server_ce->create_object = php_phongo_server_create_object;
	PHONGO_CE_FINAL(php_phongo_server_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_server_ce);

	memcpy(&php_phongo_handler_server, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(server);
	php_phongo_handler_server.get_debug_info = php_phongo_server_get_debug_info;
	php_phongo_handler_server.free_obj       = php_phongo_server_free_object;
	php_phongo_handler_server.offset         = XtOffsetOf(php_phongo_server_t, std);

	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_UNKNOWN"), PHONGO_SERVER_UNKNOWN);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_STANDALONE"), PHONGO_SERVER_STANDALONE);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_MONGOS"), PHONGO_SERVER_MONGOS);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_POSSIBLE_PRIMARY"), PHONGO_SERVER_POSSIBLE_PRIMARY);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_PRIMARY"), PHONGO_SERVER_RS_PRIMARY);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_SECONDARY"), PHONGO_SERVER_RS_SECONDARY);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_ARBITER"), PHONGO_SERVER_RS_ARBITER);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_OTHER"), PHONGO_SERVER_RS_OTHER);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_GHOST"), PHONGO_SERVER_RS_GHOST);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_LOAD_BALANCER"), PHONGO_SERVER_LOAD_BALANCER);
} /* }}} */

void phongo_server_init(zval* return_value, zval* manager, uint32_t server_id) /* {{{ */
{
	php_phongo_server_t* server;

	object_init_ex(return_value, php_phongo_server_ce);

	server            = Z_SERVER_OBJ_P(return_value);
	server->server_id = server_id;

	ZVAL_ZVAL(&server->manager, manager, 1, 0);
}
/* }}} */

bool php_phongo_server_to_zval(zval* retval, mongoc_client_t* client, mongoc_server_description_t* sd) /* {{{ */
{
	mongoc_host_list_t* host           = mongoc_server_description_host(sd);
	const bson_t*       hello_response = mongoc_server_description_hello_response(sd);
	bson_iter_t         iter;

	array_init(retval);

	ADD_ASSOC_STRING(retval, "host", host->host);
	ADD_ASSOC_LONG_EX(retval, "port", host->port);
	ADD_ASSOC_LONG_EX(retval, "type", php_phongo_server_description_type(sd));
	ADD_ASSOC_BOOL_EX(retval, "is_primary", !strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_RS_PRIMARY].name));
	ADD_ASSOC_BOOL_EX(retval, "is_secondary", !strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_RS_SECONDARY].name));
	ADD_ASSOC_BOOL_EX(retval, "is_arbiter", !strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_RS_ARBITER].name));
	ADD_ASSOC_BOOL_EX(retval, "is_hidden", bson_iter_init_find_case(&iter, hello_response, "hidden") && bson_iter_as_bool(&iter));
	ADD_ASSOC_BOOL_EX(retval, "is_passive", bson_iter_init_find_case(&iter, hello_response, "passive") && bson_iter_as_bool(&iter));

	if (bson_iter_init_find(&iter, hello_response, "tags") && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
		const uint8_t*        bytes;
		uint32_t              len;
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_DEBUG_STATE(state);
		bson_iter_document(&iter, &len, &bytes);
		if (!php_phongo_bson_to_zval_ex(bytes, len, &state)) {
			/* Exception already thrown */
			zval_ptr_dtor(&state.zchild);
			return false;
		}

		ADD_ASSOC_ZVAL_EX(retval, "tags", &state.zchild);
	}

	/* If the server description is a load balancer, its hello_response will be
	 * empty. Instead, report the hello_response from the handshake description
	 * (i.e. backing server). */
	if (!strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_LOAD_BALANCER].name)) {
		const bson_t*                handshake_response;
		mongoc_server_description_t* handshake_sd;
		bson_error_t                 error = { 0 };
		php_phongo_bson_state        state;

		if (!(handshake_sd = mongoc_client_get_handshake_description(client, mongoc_server_description_id(sd), NULL, &error))) {
			phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get handshake server description: %s", error.message);
			return false;
		}

		PHONGO_BSON_INIT_DEBUG_STATE(state);
		handshake_response = mongoc_server_description_hello_response(handshake_sd);

		if (!php_phongo_bson_to_zval_ex(bson_get_data(handshake_response), handshake_response->len, &state)) {
			/* Exception already thrown */
			mongoc_server_description_destroy(handshake_sd);
			zval_ptr_dtor(&state.zchild);
			return false;
		}

		ADD_ASSOC_ZVAL_EX(retval, "last_hello_response", &state.zchild);
		mongoc_server_description_destroy(handshake_sd);
	} else {
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_DEBUG_STATE(state);

		if (!php_phongo_bson_to_zval_ex(bson_get_data(hello_response), hello_response->len, &state)) {
			/* Exception already thrown */
			zval_ptr_dtor(&state.zchild);
			return false;
		}

		ADD_ASSOC_ZVAL_EX(retval, "last_hello_response", &state.zchild);
	}

	/* TODO: Use MONGOC_RTT_UNSET once it is added to libmongoc's public API (CDRIVER-4176) */
	if (mongoc_server_description_round_trip_time(sd) == -1) {
		ADD_ASSOC_NULL_EX(retval, "round_trip_time");
	} else {
		ADD_ASSOC_LONG_EX(retval, "round_trip_time", (zend_long) mongoc_server_description_round_trip_time(sd));
	}

	return true;
} /* }}} */
