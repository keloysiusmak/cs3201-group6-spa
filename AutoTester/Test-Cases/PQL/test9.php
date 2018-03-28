<?php
$tests = array('such that Follows', 'such that Follows*', 'such that Parent',
'such that Parent*', 'such that Uses', 'such that Modifies', 'such that Calls',
'such that Calls*', 'such that Next', 'such that Next*', 'such that Affects',
'such that Affects*', 'pattern', 'with', 'with');

$counterNamesFull = array('assign', 'stmt', 'while', 'variable', 'call', 'proc', 'prog_line', 'if', 'constant');
$intersectsName = array('a', 's', 'w', 'v', 'c', 'p', 'n', 'ifs', 'ct');

$intersects = array(
  array(0,1,2,4,6,7),
  array(0,1,2,4,6,7),
  array(0,1,2,4,6,7),
  array(0,1,2,4,6,7),
  array(0,1,2,3,4,5,6,7),
  array(0,1,2,3,4,5,6,7),
  array(5),
  array(5),
  array(0,1,2,4,6,7),
  array(0,1,2,4,6,7),
  array(0),
  array(0),
  array(0,2,3,7),
  array(3,4,5),
  array(0,1,2,4,6,7,8)
);

$clausesLHS = array(
  array(0, 1, 2, 3, 4, 5, 8, 9, 10, 11, 12, 14),
  array(0, 1, 2, 3, 4, 5, 8, 9, 14),
  array(0, 1, 2, 3, 4, 5, 8, 9, 12, 14),
  array(13),
  array(0, 1, 4, 5, 8, 9,13, 14),
  array(4,5,6,7,13),
  array(0, 1, 2, 3, 4, 5, 8, 9, 14),
  array(0, 1, 2, 3, 4, 5, 8, 9, 12, 14),
  array()
);
$clausesRHS = array(
  array(0, 1, 2, 3, 8, 9, 10, 11),
  array(0, 1, 2, 3, 8, 9),
  array(0, 1, 2, 3, 8, 9),
  array(4, 5, 12,13),
  array(0, 1, 2, 3, 8, 9,13),
  array(6,7,13),
  array(0, 1, 2, 3, 8, 9),
  array(0, 1, 2, 3, 8, 9),
  array(14)
);

$testNo = 1;
for ($i = 0; $i < sizeof($tests); $i++) {
  for ($j = 0; $j < sizeof($tests); $j++) {
    if ((sizeof(array_intersect($intersects[$i], $intersects[$j])) > 0)) {
      $counters = array(0,0,0,0,0,0,0,0,0);

      $intersected = array_filter(array_intersect($intersects[$i], $intersects[$j]));
      $n = array();
      foreach ($intersected as $value) {
        array_push($n, $value);
      }
      $intersected = $n;
      if (($i == $j) && ($j == 12)) {
          $intersected = array(3);
      }
      if (sizeof($intersected) > 0) {
        $r = mt_rand() % sizeof($intersected);
        $rr = $intersected[$r];

        $counters[$rr]++;

        $found1 = 0;
        if (in_array($i, $clausesLHS[$rr])) {
          $lhs1 = $intersectsName[$rr].$counters[$rr];
          $found1 = 1;
        }
        else {
          $canFind = array();
          $found = 0;
          for ($z = 0; $z < sizeof($clausesLHS); $z++) {
            if (in_array($i, $clausesLHS[$z])) {
              array_push($canFind, $z);
              $found++;
            }
          }
          $lr1 = mt_rand() % $found;
          $counters[$canFind[$lr1]]++;
          $lhs1 = $intersectsName[$canFind[$lr1]].$counters[$canFind[$lr1]];
        }
        if ($found1 == 1) {
          $canFind = array();
          $found = 0;
          for ($z = 0; $z < sizeof($clausesRHS); $z++) {
            if (in_array($i, $clausesRHS[$z])) {
              array_push($canFind, $z);
              $found++;
            }
          }
          $rr1 = mt_rand() % $found;
          $counters[$canFind[$rr1]]++;
          $rhs1 = $intersectsName[$canFind[$rr1]].$counters[$canFind[$rr1]];
        }
        else {
          $rhs1 = $intersectsName[$rr].$counters[$rr];
        }

        $found2 = 0;
        if (in_array($i, $clausesLHS[$rr])) {
          $lhs2 = $intersectsName[$rr].$counters[$rr];
          $found2 = 1;
        }
        else {

          $canFind = array();
          $found = 0;
          for ($z = 0; $z < sizeof($clausesLHS); $z++) {
            if (in_array($i, $clausesLHS[$z])) {
              array_push($canFind, $z);
              $found++;
            }
          }
          $lr2 = mt_rand() % $found;
          if (sizeof($canFind) > 1) {
            while ($lr2 == $lr1) {
              $lr2 = mt_rand() % $found;
            }
          }

          $counters[$canFind[$lr2]]++;
          $lhs2 = $intersectsName[$canFind[$lr2]].$counters[$canFind[$lr2]];
        }
        if ($found2 == 1) {


            $canFind = array();
            $found = 0;
            for ($z = 0; $z < sizeof($clausesRHS); $z++) {
              if (in_array($j, $clausesRHS[$z])) {
                array_push($canFind, $z);
                $found++;
              }
            }
            $rr2 = mt_rand() % $found;
            if (sizeof($canFind) > 1) {
              while ($rr2 == $rr1) {
                $rr2 = mt_rand() % $found;
              }
            }

            $counters[$canFind[$rr2]]++;
            $rhs2 = $intersectsName[$canFind[$rr2]].$counters[$canFind[$rr2]];
        }
        else {
          $rhs2 = $intersectsName[$rr].$counters[$rr];
        }
        $thisParams = "";
        $thisDeclarations = "";
        $x = 0;
        while ($x <= 8) {
          if ($counters[$x] == 3) {
            $thisDeclarations .= $counterNamesFull[$x]." ".$intersectsName[$x]."1, ".$intersectsName[$x]."2, ".$intersectsName[$x]."3; ";
            $thisParams = $intersectsName[$x]."1, ".$intersectsName[$x]."2, ".$intersectsName[$x]."3, ";
          }
          else if ($counters[$x] == 2) {
            $thisDeclarations .= $counterNamesFull[$x]." ".$intersectsName[$x]."1, ".$intersectsName[$x]."2; ";
            $thisParams .= $intersectsName[$x]."1, ".$intersectsName[$x]."2, ";
          }
          else if ($counters[$x] == 1) {
            if ($thisParams == "") {
              $thisDeclarations .= $counterNamesFull[$x]." ".$intersectsName[$x]."1; ";
              $thisParams = $intersectsName[$x].$counters[$x].", ";
            }
            else {
              $thisDeclarations .= $counterNamesFull[$x]." ".$intersectsName[$x]."1; ";
              $thisParams .= $intersectsName[$x]."1, ";
            }
          }
          $x++;
        }
        print "$testNo - <br/>";
        print $thisDeclarations."<br/>";
        print "Select &#60;".substr($thisParams, 0, strlen($thisParams) - 2)."&#62; ";
        if ($i < 12) {
          print $tests[$i]."($lhs1, $rhs1) ";
        }
        else if ($i == 12){
          $extra = "";
          if (substr($lhs1, 0, 3) == "ifs") {
            $extra = ", _";
          }
          print $tests[$i]." ".$lhs1."($rhs1, _$extra) ";
        }
        else if ($i == 13){
          print $tests[$i]." ".processWith($lhs1)." = ".processWith($rhs1)." ";
        }
        else if ($i == 14){
          print $tests[$i]." ".processWith2($lhs1)." = ".processWith2($rhs1)." ";
        }
        if ($j < 12) {
          print $tests[$j]."($lhs2, $rhs2)<br/>";
        }
        else if ($j == 12){
          $extra = "";
          if (substr($lhs2, 0, 3) == "ifs") {
            $extra = ", _";
          }
          print $tests[$j]." ".$lhs2."($rhs2, _$extra)<br/>";
        }
        else if ($j == 13){
          print $tests[$j]." ".processWith($lhs2)." = ".processWith($rhs2)." <br/>";
        }
        else if ($j == 14){
          print $tests[$j]." ".processWith2($lhs2)." = ".processWith2($rhs2)." <br/>";
        }
        print "</br>";
        print "5000</br>";
        $testNo++;
      }
    }
  }
}

function processWith($a) {
  if (substr($a, 0, 1) == "c") {
    return $a.".procName";
  } else if (substr($a, 0, 1) == "v") {
    return $a.".varName";
  } else if (substr($a, 0, 1) == "p") {
    return $a.".procName";
  } else {
    return $a.".stmt#";
  }
}

function processWith2($a) {
  if (substr($a, 0, 2) == "ct") {
    return $a.".value";
  } else {
    return $a.".stmt#";
  }
}
?>
