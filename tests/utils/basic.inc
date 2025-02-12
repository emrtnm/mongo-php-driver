<?php

require_once __DIR__ . "/" . "tools.php";

define('URI', getenv('MONGODB_URI') ?: 'mongodb://127.0.0.1/');
define('MONGO_ORCHESTRATION_URI', getenv('MONGO_ORCHESTRATION_URI') ?: 'http://localhost:8889/v1');
define('DATABASE_NAME', getenv('MONGODB_DATABASE') ?: 'phongo');
define('COLLECTION_NAME', makeCollectionNameFromFilename($_SERVER['SCRIPT_FILENAME']));
define('NS', DATABASE_NAME . '.' . COLLECTION_NAME);
define('SSL_DIR', realpath(getenv('SSL_DIR')));
define('CSFLE_KEY_VAULT_NS', 'encryption.__keyVault');
define('CSFLE_LOCAL_KEY', base64_decode('Mng0NCt4ZHVUYUJCa1kxNkVyNUR1QURhZ2h2UzR2d2RrZzh0cFBwM3R6NmdWMDFBMUN3YkQ5aXRRMkhGRGdQV09wOGVNYUMxT2k3NjZKelhaQmRCZGJkTXVyZG9uSjFk'));
