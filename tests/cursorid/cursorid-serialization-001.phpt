--TEST--
MongoDB\Driver\CursorId serialization (Serializable interface)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_php_version('>=', '7.4.0'); ?>
--FILE--
<?php

require_once __DIR__ . "/../utils/basic.inc";

$tests = [
    unserialize('C:23:"MongoDB\Driver\CursorId":42:{a:1:{s:2:"id";s:19:"7250031947823432848";}}'),
    unserialize('C:23:"MongoDB\Driver\CursorId":23:{a:1:{s:2:"id";s:1:"0";}}'),
];

foreach ($tests as $test) {
    var_dump($test);
    var_dump($test instanceof Serializable);
    echo $s = serialize($test), "\n";
    var_dump(unserialize($s));
    echo "\n";
}

?>
===DONE===
<?php exit(0); ?>
--EXPECTF--
object(MongoDB\Driver\CursorId)#%d (%d) {
  ["id"]=>
  %rint\(7250031947823432848\)|string\(19\) "7250031947823432848"%r
}
bool(true)
C:23:"MongoDB\Driver\CursorId":42:{a:1:{s:2:"id";s:19:"7250031947823432848";}}
object(MongoDB\Driver\CursorId)#%d (%d) {
  ["id"]=>
  %rint\(7250031947823432848\)|string\(19\) "7250031947823432848"%r
}

object(MongoDB\Driver\CursorId)#%d (%d) {
  ["id"]=>
  int(0)
}
bool(true)
C:23:"MongoDB\Driver\CursorId":23:{a:1:{s:2:"id";s:1:"0";}}
object(MongoDB\Driver\CursorId)#%d (%d) {
  ["id"]=>
  int(0)
}

===DONE===
