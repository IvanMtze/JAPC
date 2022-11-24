{OK}
program vars;

var
  a,b,c,d:Integer;
  aa,bb,cc,dd:Real;
  a,c:Integer; {-Duplicated variables same type}
  c,d:Real; {Duplicated variables but different type}
  dda:type of a; {OK-Duplicated variables but different type}
  ddaa:type of dda; {OK}
begin
{EMPTY BODY OK}
end.