<?php
  $numberOfLines = 45;
  $tests = array('Follows', 'Follows*', 'Parent', 'Parent*', 'Uses', 'Modifies', 'Calls', 'Calls*',
'Next', 'Next*', 'Affects', 'Affects*', 'pattern', 'with', 'with');

  $paramLHS = array(
    array(1, 4, 41, "a", "c", "s"),
    array(1, 4, 41, "a", "c", "s"),
    array(8, 31, "w", "if"),
    array(8, 31, "w", "if"),
    array("\"A\"", "\"B\"", "s", "c", 8,31),
    array("\"A\"", "\"B\"", "s", "c", 8,31),
    array("A","B"),
    array("A","B"),
    array(1,8,14,22,31,37,"s"),
    array(1,8,14,22,31,37,"s"),
    array(1,9,17,25,32,"a"),
    array(1,9,17,25,32,"a"),
    array("a", "ifs", "w"),
    array("c", "p", "v"),
    array("a", "if", "w", "n", "s", "c")
  );

  $paramRHS = array(
    array(2, 5, 51, "a", "s", "w"),
    array(2, 5, 51, "a", "s", "w"),
    array(14, 17, 35, 52, "s", "a"),
    array(14, 17, 35, 52, "s", "a"),
    array("v", "a1","e1","z"),
    array("v", "b1","c1","z"),
    array("A","B"),
    array("A","B"),
    array(1,8,14,22,31,37,"s"),
    array(1,8,14,22,31,37,"s"),
    array(1,9,17,25,32,"a"),
    array(1,9,17,25,32,"a"),
    array("v"),
    array("c", "p", "v"),
    array("ct", 7, 14)
  );

  $testNo = 1;
  $counterC = 0;
  foreach($tests as $thisTest) {
    $lhs = $paramLHS[$counterC];
    $rhs = $paramRHS[$counterC];
    $arguments = array();
    $declarations = array();
    $parameters = array();
    foreach ($lhs as $thisLHS) {
      $counterNamesFull = array('assign', 'stmt', 'while', 'variable', 'call', 'proc', 'prog_line', 'if', 'constant');
      $counterNames = array('a', 's', 'w', 'v', 'c', 'p', 'n', 'ifs', 'ct');
      $initCounter = array(0,0,0,0,0,0,0,0,0);

      $i = 0;
      while ($i <= 8) {
        if ($thisLHS == $counterNames[$i]) {
          $initCounter[$i]++;
          $thisLHS = $thisLHS.$initCounter[$i];
        }
        $i++;
      }
      foreach ($rhs as $thisRHS) {
        $counter = $initCounter;
        $i = 0;
        while ($i <= 8) {
          if ($thisRHS == $counterNames[$i]) {
            $counter[$i]++;
            $thisRHS = $thisRHS.$counter[$i];
          }
          $i++;
        }
        $i = 0;
        $thisParams = "";
        $thisDeclarations = "";
        while ($i <= 8) {
          if ($counter[$i] == 2) {
            $thisDeclarations = $counterNamesFull[$i]." ".$counterNames[$i]."1,".$counterNames[$i]."2;";
            $thisParams = "&#60;".$counterNames[$i]."1,".$counterNames[$i]."2&#62;";
          }
          else if ($counter[$i] == 1) {
            if ($thisParams == "") {
              $thisDeclarations .= $counterNamesFull[$i]." ".$counterNames[$i]."1; ";
              $thisParams = $counterNames[$i].$counter[$i];
            }
            else {
              $thisDeclarations .= $counterNamesFull[$i]." ".$counterNames[$i]."1; ";
              $thisParams = "&#60;".$counterNames[$i]."1,".$thisParams."&#62;";
            }
          }
          $i++;
        }
        if ($thisParams == "") {
          $thisParams = "BOOLEAN";
        }

        array_push($declarations, $thisDeclarations);
        array_push($parameters, $thisParams);
        if ($counterC < 12) {
          array_push($arguments, "$thisLHS,$thisRHS");
        }
        else if ($counterC == 12) {
          $extra = "";
          if (substr($thisLHS, 0, 3) == "ifs") {
            $extra = ", _";
          }
          array_push($arguments, "$thisLHS($thisRHS, _$extra)");
        }
        else if ($counterC == 13) {
          array_push($arguments, processWith($thisLHS)." = ".processWith($thisRHS));
        }
        else if ($counterC == 14) {
          array_push($arguments, processWith2($thisLHS)." = ".processWith2($thisRHS));
        }
      }
    }
    $i = 0;
    foreach ($arguments as $thisArgument) {
      print "$testNo - <br/>";
      print $declarations[$i]."<br/>";
      $args = "";
      if ($counterC < 12) {
        $args = "such that $thisTest($thisArgument)";
      }
      else if ($counterC >= 12) {
        $args = "$thisTest $thisArgument";
      }
      print "Select ".$parameters[$i]." $args;<br/>";//calsuse
      print "</br>";
      print "5000</br>";
      $testNo++;
      $i++;
    }
    $counterC++;
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
    } else if (is_numeric($a)) {
      return $a;
    } else {
      return $a.".stmt#";
    }
  }
?>
