<?php

/*
procedure a {
1.  while c {
2.    while c {
3.      x = x + 1;
4.      x = 1;
5.      x = x + 1;
6.      x = 1;}}}
*/

$testCounter = 1;

$rship = array(
  'Follows', 'Follows*', 'Parent', 'Parent*'
);

$all = "1,2,3,4,5,6";
$notAll = "";

$rcount = 0;
foreach ($rship as $r) {
  for($i = 1; $i <= 6; $i++) {
    for($j = 1; $j <= 6; $j++) {
      $res = $notAll;
      if ($rcount == 0) {
        if (($i >= 3) && (($i + 1) == $j)) {
          $res = $all;
        }
      } else if ($rcount == 1) {
        if (($i >= 3) && ($i < $j)) {
          $res = $all;
        }
      } else if ($rcount == 2) {
        if (($i == 1) && ($j == 2)) {
          $res = $all;
        } else if (($i == 2) && ($j > $i)) {
          $res = $all;
        }
      } else if ($rcount == 3) {
        if (($i == 1) && ($j > $i)) {
          $res = $all;
        } else if (($i == 2) && ($j > $i)) {
          $res = $all;
        }
      }
      print "$testCounter - $r($i, $j)<br/>
      stmt s;<br/>
      Select s such that $r($i, $j)<br/>
      ".$res."<br/>
      5000<br/>";
      $testCounter++;
    }
  }
  $rcount++;
}

$rship = array(
  'Uses', 'Modifies'
);

$all = "1,2,3,4,5,6";
$notAll = "";

$rcount = 0;
foreach ($rship as $r) {
  for($i = 1; $i <= 6; $i++) {
    $res = $notAll;
    if ($rcount == 0) {
      if (($i == 2) || ($i % 2 == 1)) {
        $res = $all;
      }
    } else if ($rcount == 1) {
      $res = $all;
    }
    print "$testCounter - $r($i, $j)<br/>
    stmt s;<br/>
    Select s such that $r($i, \"x\")<br/>
    ".$res."<br/>
    5000<br/>";
    $testCounter++;
  }
  $rcount++;
}

$rship = array(
  'Uses', 'Modifies'
);

$all = "3,4,5,6";
$uses = "3,5";

$patternRHS = array(
  '_"1"_', '_"x"_', '_'
);

$rcount = 0;
foreach ($rship as $r) {
  for($i = 0; $i < 3; $i++) {
    $res = $uses;
    if ($rcount == 0) {
      if (($i == 0)|| ($i == 2)) {
        $res = $all;
      }
    } else if ($rcount == 1) {
      if (($i == 0)|| ($i == 2)) {
        $res = $all;
      }
    }
    print "$testCounter - $r($i, $j)<br/>
    assign a; variable v;<br/>
    Select a such that $r(a,v) pattern a(v, ".$patternRHS[$i].")<br/>
    ".$res."<br/>
    5000<br/>";
    $testCounter++;
  }
  $rcount++;
}
?>
