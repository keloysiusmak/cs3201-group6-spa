<?php

/*
procedure a {
1.    while z {
2.        while z {
3.            while z {
4.                while z {
5.                    while z {
6.                        z = x + 1;
7.                        z = x + 2;
8.                        z = x + 3;}}}}}
9.     while z {
10.       while z {
11.           while z {
12.               while z {
13.                   if z then {
14.                       z = x + 1;
15.                       z = x + 2;
16.                       z = x + 3;
                      } else {
17.                       a = b;}}}}}
}
*/

$allStatements = array(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17);
$allStatementsUsesZ = array(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
$allZ = array(6,7,8,14,15,16);
$allZ1 = array(6,14);
$allAssign = array(6,7,8,14,15,16,17);
$allAssignFollows = array(6,7,14,15);
$allContainers = array(1,2,3,4,5,9,10,11,12,13);

$testCounter = 1;
$inputs = array(
  'Follows(0,1)', 'Follows(1,0)', 'Follows("a", 1)', 'Follows(1,"a")', 'Follows("a", "a")',
  'Follows*(0,1)', 'Follows*(1,0)', 'Follows*("a", 1)', 'Follows*(1,"a")', 'Follows*("a", "a")',
  'Parent(0,1)', 'Parent(1,0)', 'Parent("a", 1)', 'Parent(1,"a")', 'Parent("a", "a")',
  'Parent*(0,1)', 'Parent*(1,0)', 'Parent*("a", 1)', 'Parent*(1,"a")', 'Parent*("a", "a")',
  'Uses(0,1)', 'Uses(1,0)', 'Uses("a", 1)', 'Uses(_,1)', 'Uses("a", "a")', 'Uses(_, "a")',
  'Modifies(0,1)', 'Modifies(1,0)', 'Modifies("a", 1)', 'Modifies(_,1)', 'Modifies("a", "a")', 'Modifies(_, "a")'
);

foreach($inputs as $i) {
  print $testCounter." - ".str_replace("\"", "'", $i)."<br/>
  stmt s;<br/>
  Select s such that ".$i."<br/><br/>
  5000<br/>";
  $testCounter++;
}

$secondSet = array(
  'Follows(a1,a2)', 'Parent(s,a1)', 'Uses(a1,"x")', 'Modifies(a1,"z")'
);
$patternSetLHS = array(
  '_', 'v', '"z"'
);
$patternSetRHS = array(
  '_', '_"1"_', '_"x"_'
);
$patternSetAnswers[0] = array(
  $allAssignFollows, $allZ1, $allAssignFollows, $allAssignFollows, $allZ1, $allAssignFollows, $allAssignFollows, $allZ1, $allAssignFollows
);
$patternSetAnswers[1] = array(
  $allAssign, $allZ1, $allZ, $allAssign, $allZ1, $allZ, $allZ, $allZ1, $allZ
);
$patternSetAnswers[2] = array(
  $allZ, $allZ1, $allZ, $allZ, $allZ1, $allZ, $allZ, $allZ1, $allZ
);
$patternSetAnswers[3] = array(
  $allZ, $allZ1, $allZ, $allZ, $allZ1, $allZ, $allZ, $allZ1, $allZ
);
$vars = array(
  "assign a1, a2;",
  "stmt s; assign a1;",
  "assign a1;",
  "assign a1;",
);

for($a = 0; $a < count($secondSet); $a++) {
  for($b = 0; $b < count($patternSetLHS); $b++) {
    for($c = 0; $c < count($patternSetRHS); $c++) {
      print $testCounter." - ".str_replace("\"", "'", $secondSet[$a])." pattern a1(".str_replace("\"", "'", $patternSetLHS[$b]).", ".str_replace("\"", "'", $patternSetRHS[$c]).")<br/>
      ".$vars[$a]."<br/>
      Select a1 such that ".$secondSet[$a]." pattern a1(".$patternSetLHS[$b].", ".$patternSetRHS[$c].")<br/>".implode(",", $patternSetAnswers[$a][$b * 3 + $c])."<br/>
      5000<br/>";
      $testCounter++;
    }
  }
}

$thirdSet = array(
  'Uses(s,v)', 'Modifies(s,v)'
);
$patternSetAnswers[0] = array(
  $allZ, $z1, $allZ
);
$patternSetAnswers[1] = array(
  $allZ, $z1, $allZ
);

for($a = 0; $a < count($thirdSet); $a++) {
  for($b = 0; $b < count($patternSetRHS); $b++) {
    print $testCounter." - ".str_replace("\"", "'", $thirdSet[$a])." pattern a1(v, ".str_replace("\"", "'", $patternSetRHS[$b]).")<br/>
    stmt s; assign a1;<br/>Select a1 such that ".$thirdSet[$a]." pattern a1(v, ".$patternSetRHS[$b].")<br/>".implode(",", $patternSetAnswers[$a][$b])."<br/>
    5000<br/>";
    $testCounter++;
  }
}
?>
