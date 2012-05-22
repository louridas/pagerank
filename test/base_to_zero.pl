use strict;

while (<>) {
  my @nodes = map {$_ - 1 } split;
  print "@nodes" . "\n";
}
