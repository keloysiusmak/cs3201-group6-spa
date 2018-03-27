<?php

$procedureCount = 0;
$ifCount = 0;
$elseCount = 0;
$whileCount = 0;
$tab = "&nbsp;&nbsp;&nbsp;&nbsp;";

$procedureNames = array("First", "Second", "Third", "Fourth", "Fifth");
$currProcedure = 0;
$maxProcedures = sizeof($procedureNames);
$level = 0;

$stack = array();

$numberOfLines = 200;
$totalLines = $numberOfLines;
while ($currProcedure < $maxProcedures) {
  while ($numberOfLines > 0) {
    if (($procedureCount == 0) && (sizeof($stack) == 0) && ($ifCount == 0) && ($elseCount == 0) && ($whileCount == 0)) {
      print "procedure ".$procedureNames[$currProcedure]." {<br/>";
      $currProcedure++;
      $procedureCount++;
      $level++;
      $procedureStatements = 0;
    }
    $toHaveLine = mt_rand()%5;
    if (($currProcedure == $maxProcedures) || ($toHaveLine > 1)) {
      $rand = mt_rand()%15 + 1;
      if ($rand <= 12) {
        lines($level, $totalLines);
        $numberOfLines--;
        $procedureStatements++;
        if ($stack[sizeof($stack) - 1] == "if") {
          $ifStatements++;
        } else if ($stack[sizeof($stack) - 1] == "else") {
          $elseStatements++;
        } else if ($stack[sizeof($stack) - 1] == "while") {
          $whileStatements++;
        }
      } else if ($rand <= 13) {
        $rand2 = rand()%2 + 1;
        if (($stack[sizeof($stack) - 1] == "if") && ($ifStatements > 0) && ($rand2 == 2)) {
          $level--;
          for($i = 0; $i < $level; $i++) {
            print $tab;
          }
          print "}<br/>";
          for($i = 0; $i < $level; $i++) {
            print $tab;
          }
          $level++;
          print "else {<br/>";
          $elseStatements = 0;
          $ifCount--;
          $elseCount++;
          $stack[sizeof($stack) - 1] = "else";
        }
        else {
          for($i = 0; $i < $level; $i++) {
            print $tab;
          }
          print "if a then {<br/>";
          $ifCount++;
          $stack[] = "if";
          $ifStatements = 0;
          $level++;
        }
      } else if ($rand <= 14) {
        for($i = 0; $i < $level; $i++) {
          print $tab;
        }
        $whileCount++;
        $whileStatements = 0;
        print "while a {<br/>";
        $level++;
        $stack[] = "while";
      } else if (($rand <= 15) && ($currProcedure != sizeof($procedureNames))) {
        for($i = 0; $i < $level; $i++) {
          print $tab;
        }
        print "call ";
        proc($currProcedure-1, $procedureNames);
        print ";<br/>";
        $numberOfLines--;
        $procedureStatements++;
        if ($stack[sizeof($stack) - 1] == "if") {
          $ifStatements++;
        } else if ($stack[sizeof($stack) - 1] == "else") {
          $elseStatements++;
        } else if ($stack[sizeof($stack) - 1] == "while") {
          $whileStatements++;
        }
      }
    }
    else {
      if ((($stack[sizeof($stack) - 1] == "while") && ($whileCount > 0) && ($whileStatements > 0))) {
        $level--;
        for($i = 0; $i < $level; $i++) {
          print $tab;
        }
        print "}<br/>";
        array_pop($stack);
        $whileCount--;
      }
      else if (($stack[sizeof($stack) - 1] == "else") && ($elseCount > 0) && ($elseStatements > 0)) {
        $level--;
        for($i = 0; $i < $level; $i++) {
          print $tab;
        }
        print "}<br/>";
        $elseCount--;
        array_pop($stack);
      }
      else if (($stack[sizeof($stack) - 1] == "if") && ($ifCount > 0) && ($ifStatements > 0)) {
        $level--;
        for($i = 0; $i < $level; $i++) {
          print $tab;
        }
        print "}<br/>";
        for($i = 0; $i < $level; $i++) {
          print $tab;
        }
        $level++;
        print "else {<br/>";
        $elseStatements = 0;
        $stack[sizeof($stack) - 1] = "else";
        $ifCount--;
        $elseCount++;
      }
      else if ((($procedureStatements > ($totalLines / $maxProcedures))) && ($ifCount == 0) && ($elseCount == 0) && ($whileCount == 0) && ($procedureCount > 0)) {
        $level--;
        for($i = 0; $i < $level; $i++) {
          print $tab;
        }
        print "}<br/>";
        $procedureCount--;
      }
    }
  }
  if ($currProcedure < $maxProcedures) {
    $numberOfLines += ($totalLines / $maxProcedures);
  }
}

function many_lines($level, $max, $totalLines) {
  $newrand = mt_rand()%$max + 1;
  while ($newrand > 0) {
    lines($level, $totalLines);
    $newrand--;
  }
}
function variable() {
  $r = mt_rand() % 26;
  print chr($r+97);
}
function lines($level, $totalLines) {
  for($i = 0; $i < $level; $i++) {
    print "&nbsp;&nbsp;&nbsp;&nbsp;";
  }
  $noOfUses = rand()%6 + 2;
  $set = 1;
  while ($noOfUses > 0) {
    $r3 = mt_rand() % 9 + 1;
    if (($set == 1) || ($r3 > 1)) {
      variable();
    }
    else {
      print mt_rand() % $totalLines + 1;
    }
    if ($set == 1) {
      print " = ";
      $set = 0;
    }
    else if ($noOfUses > 1){
        $r2 = mt_rand() % 3;
        if ($r2 == 0) {
          print " + ";
        } else if ($r2 == 1) {
          print " * ";
        } else if ($r2 == 2) {
          print " - ";
        }
    }
    $noOfUses--;
  }
  print ";";
  print "<br/>";
}
function proc($curr,$procedureNames) {
  $size = sizeof($procedureNames);
  $r = mt_rand() % ($size - $curr - 1) + $curr + 1;
  print $procedureNames[$r];
}

while (sizeof($stack) > 0) {
  $a = $stack[sizeof($stack) - 1];
  array_pop($stack);
  if ($a == "if") {
    many_lines($level, ceil($totalLines / $maxProcedures), $totalLines);
    $level--;
    for($i = 0; $i < $level; $i++) {
      print $tab;
    }
    print "}<br/>";
    for($i = 0; $i < $level; $i++) {
      print $tab;
    }
    print "else {<br/>";
    $level++;
    array_push($stack, "else");
  } else if ($a == "while") {
    many_lines($level, ceil($totalLines / $maxProcedures), $totalLines);
    $level--;
    for($i = 0; $i < $level; $i++) {
      print $tab;
    }
    print "}<br/>";
  } else if ($a == "else") {
    many_lines($level, ceil($totalLines / $maxProcedures), $totalLines);
    $level--;
    for($i = 0; $i < $level; $i++) {
      print $tab;
    }
    print "}<br/>";
  }
}
print "}";
?>
