program constants;

const
  a:Integer; {Expected '='}
  a = 100; {OK?}
  b = 'sdfs'; {OK?}
  k = "sdfs"; {OK?}
  c = a + b; {Error when type checking}
  d = a - d; { undefined d }
begin

end.