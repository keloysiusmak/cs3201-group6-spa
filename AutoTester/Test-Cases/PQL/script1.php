<?php

/*
procedure a {
1.    x = x + 1;
2.    if x then {
3.        if x then {
4.            x = x + 1;
          } else {
5.            x = x + 1; }
6.        while x {
7.            x = x + 1; }
      } else {
8.        x = x; }
9.    while x {
10.       while x {
11.           x = x + 1; }
12.       if x then {
13.           x = x + 1;
          } else {
14.           x = x + 1; }
15        x = x + 1; } }
}
*/
$testCounter = 1;
/* Follows */
$names1 = array("stmt", "assign", "if", "while", "prog_line");
$names2 = array("stmt", "assign", "if", "while", "prog_line");
$generic1 = array("s1", "a1", "if1", "w1", "n1", "_");
$generic2 = array("s2", "a2", "if2", "w2", "n2", "_");

$c = count($generic1);
$d = count($generic2);
for ($i = 0; $i < $c; $i++) {
  for ($j = 0; $j < $d; $j++) {
    if (!($i == ($c - 1) && $j == ($d - 1))) {
      print $testCounter." - Follows(".$generic1[$i].", ".$generic2[$j].")<br/>";
      if ($i < ($c - 1)) {
        print $names1[$i]." ".$generic1[$i];
        if ($i == $j) {
          print ", ".$generic2[$i].";<br/>";
        } else {
          if ($j < ($d - 1)) {
            print "; ".$names2[$j]." ".$generic2[$j].";<br/>";
          } else {
            print "; <br/>";
          }
        }
      } else {
        print $names2[$j]." ".$generic2[$j].";<br/>";
      }
      $g = $generic1[$i];
      if ($i == ($c - 1)) {
        $g = $generic2[$j];
      }
      print "Select ".$g." such that Follows(".$generic1[$i].", ".$generic2[$j].")<br/>";
      print "<br/>";
      print "5000<br/>";
      $testCounter++;
    }
  }
}

for ($i = 0; $i < $c; $i++) {
  for ($j = 0; $j < $d; $j++) {
    if (!($i == ($c - 1) && $j == ($d - 1))) {
      print $testCounter." - Follows*(".$generic1[$i].", ".$generic2[$j].")<br/>";
      if ($i < ($c - 1)) {
        print $names1[$i]." ".$generic1[$i];
        if ($i == $j) {
          print ", ".$generic2[$i].";<br/>";
        } else {
          if ($j < ($d - 1)) {
            print "; ".$names2[$j]." ".$generic2[$j].";<br/>";
          } else {
            print "; <br/>";
          }
        }
      } else {
        print $names2[$j]." ".$generic2[$j].";<br/>";
      }
      $g = $generic1[$i];
      if ($i == ($c - 1)) {
        $g = $generic2[$j];
      }
      print "Select ".$g." such that Follows*(".$generic1[$i].", ".$generic2[$j].")<br/>";
      print "<br/>";
      print "5000<br/>";
      $testCounter++;
    }
  }
}

/* Parent */
$names1 = array("stmt", "if", "while");
$names2 = array("stmt", "if", "while", "assign", "prog_line");
$generic1 = array("s1", "if1", "w1", "_");
$generic2 = array("s2", "if2", "w2", "a2", "n2", "_");

$c = count($generic1);
$d = count($generic2);
for ($i = 0; $i < $c; $i++) {
  for ($j = 0; $j < $d; $j++) {
    if (!($i == ($c - 1) && $j == ($d - 1))) {
      print $testCounter." - Parent(".$generic1[$i].", ".$generic2[$j].")<br/>";
      if ($i < ($c - 1)) {
        print $names1[$i]." ".$generic1[$i];
        if ($i == $j) {
          print ", ".$generic2[$i].";<br/>";
        } else {
          if ($j < ($d - 1)) {
            print "; ".$names2[$j]." ".$generic2[$j].";<br/>";
          } else {
            print "; <br/>";
          }
        }
      } else {
        print $names2[$j]." ".$generic2[$j].";<br/>";
      }
      $g = $generic1[$i];
      if ($i == ($c - 1)) {
        $g = $generic2[$j];
      }
      print "Select ".$g." such that Parent(".$generic1[$i].", ".$generic2[$j].")<br/>";
      print "<br/>";
      print "5000<br/>";
      $testCounter++;
    }
  }
}

for ($i = 0; $i < $c; $i++) {
  for ($j = 0; $j < $d; $j++) {
    if (!($i == ($c - 1) && $j == ($d - 1))) {
      print $testCounter." - Parent*(".$generic1[$i].", ".$generic2[$j].")<br/>";
      if ($i < ($c - 1)) {
        print $names1[$i]." ".$generic1[$i];
        if ($i == $j) {
          print ", ".$generic2[$i].";<br/>";
        } else {
          if ($j < ($d - 1)) {
            print "; ".$names2[$j]." ".$generic2[$j].";<br/>";
          } else {
            print "; <br/>";
          }
        }
      } else {
        print $names2[$j]." ".$generic2[$j].";<br/>";
      }
      $g = $generic1[$i];
      if ($i == ($c - 1)) {
        $g = $generic2[$j];
      }
      print "Select ".$g." such that Parent*(".$generic1[$i].", ".$generic2[$j].")<br/>";
      print "<br/>";
      print "5000<br/>";
      $testCounter++;
    }
  }
}

/* Uses */
$names1 = array("stmt", "assign", "if", "while", "prog_line", "proc");
$names2 = array("variable");
$generic1 = array("s1", "a1", "if1", "w1", "n1", "p", "_");
$generic2 = array("v", "_");

$c = count($generic1);
$d = count($generic2);
for ($i = 0; $i < $c; $i++) {
  for ($j = 0; $j < $d; $j++) {
    if (!($i == ($c - 1) && $j == ($d - 1))) {
      print $testCounter." - Uses(".$generic1[$i].", ".$generic2[$j].")<br/>";
      if ($i < ($c - 1)) {
        print $names1[$i]." ".$generic1[$i];
        if ($j < ($d - 1)) {
          print "; ".$names2[$j]." ".$generic2[$j].";<br/>";
        } else {
          print "; <br/>";
        }
      } else {
        print $names2[$j]." ".$generic2[$j].";<br/>";
      }
      $g = $generic1[$i];
      if ($i == ($c - 1)) {
        $g = $generic2[$j];
      }
      print "Select ".$g." such that Uses(".$generic1[$i].", ".$generic2[$j].")<br/>";
      print "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15<br/>";
      print "5000<br/>";
      $testCounter++;
    }
  }
}

/* Modifies */
$names1 = array("stmt", "assign", "prog_line", "proc");
$names2 = array("variable");
$generic1 = array("s1", "a1", "n1", "p", "_");
$generic2 = array("v", "_");

$c = count($generic1);
$d = count($generic2);
for ($i = 0; $i < $c; $i++) {
  for ($j = 0; $j < $d; $j++) {
    if (!($i == ($c - 1) && $j == ($d - 1))) {
      print $testCounter." - Modifies(".$generic1[$i].", ".$generic2[$j].")<br/>";
      if ($i < ($c - 1)) {
        print $names1[$i]." ".$generic1[$i];
        if ($j < ($d - 1)) {
          print "; ".$names2[$j]." ".$generic2[$j].";<br/>";
        } else {
          print "; <br/>";
        }
      } else {
        print $names2[$j]." ".$generic2[$j].";<br/>";
      }
      $g = $generic1[$i];
      if ($i == ($c - 1)) {
        $g = $generic2[$j];
      }
      print "Select ".$g." such that Modifies(".$generic1[$i].", ".$generic2[$j].")<br/>";
      print "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15<br/>";
      print "5000<br/>";
      $testCounter++;
    }
  }
}

/* Pattern */
$generic1 = array("v", "\"x\"", "_");
$generic2 = array("_v_", "_\"x\"_", "_\"1\"_", "_");

$c = count($generic1);
$d = count($generic2);
for ($i = 0; $i < $c; $i++) {
  for ($j = 0; $j < $d; $j++) {
    if (!($i == ($c - 1) && $j == ($d - 1))) {
      print $testCounter." - pattern a(".$generic1[$i].", ".$generic2[$j].")<br/>";
      if ($i == 0 || $j == 0) {
        print "assign a; variable v;<br/>";
      } else {
        print "assign a;<br/>";
      }
      print "Select a pattern a(".$generic1[$i].", ".$generic2[$j].")<br/>";
      print "1,4,5,7,8,11,13,14,15<br/>";
      print "5000<br/>";
      $testCounter++;
    }
  }
}

$testCounter = 150;
$cond = array("Follows(1,2)", "Parent(2,3)", "Uses(1, \"x\")", "Modifies(1, \"x\")");
$var = array("v", "a1", "s", "ifs", "w", "p", "n");
$vars = array("variable v", "assign a, a1", "stmt s", "if ifs", "while w", "proc p", "prog_line n");
$answers = array('x', '1,4,5,7,8,11,13,14,15', '1,2,3,4,5,6,7,8,9,10,11,12,13,14,15', '2,3,12', '6,9,10', 'a', '1,2,3,4,5,6,7,8,9,10,11,12,13,14,15');
for ($i = 0; $i < 4; $i++) {
  for ($j = 0; $j < 7; $j++) {
    $out = $vars[$j]."; assign a;";
    if ($j == 1) {
      $out = "assign a, a1;";
    }
    print $testCounter." - ".$vars[$j]."<br/>
    $out<br/>
    Select ".$var[$j]." such that ".$cond[$i]." pattern a(\"x\", _\"x\"_);<br/>
    ".$answers[$j]."<br/>
    5000<br/>";
    $testCounter++;
  }
}

$cond = array("variable v", "assign a", "stmt s", "if ifs", "while w", "proc p", "prog_line n");
$cond2 = array("v", "a", "s", "ifs", "w", "p", "n");
for ($i = 0; $i < 7; $i++) {
  print $testCounter." - no clause(".($i+1).")<br/>
  ".$cond[$i].";<br/>
  Select ".$cond2[$i].";<br/>
  <br/>
  5000<br/>";
  $testCounter++;
}
?>
