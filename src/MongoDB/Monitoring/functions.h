/*
 * Copyright 2016-present MongoDB, Inc.
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

#ifndef PHONGO_MONITORING_FUNCTIONS_H
#define PHONGO_MONITORING_FUNCTIONS_H

#include <php.h>

PHP_FUNCTION(MongoDB_Driver_Monitoring_addSubscriber);
PHP_FUNCTION(MongoDB_Driver_Monitoring_removeSubscriber);

#endif /* PHONGO_MONITORING_FUNCTIONS_H */
