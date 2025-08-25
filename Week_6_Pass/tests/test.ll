declare i32 @fib(i32)
declare void @print(i32)


define i32 @complex(i32 %0) {
entry:
  %1 = alloca i32
  store i32 %0, i32* %1
  %2 = load i32, i32* %1
  switch i32 %2, label %sw.default [
    i32 0, label %sw.zero
    i32 1, label %sw.one
    i32 2, label %sw.two
  ]

sw.zero:
  br label %loop.outer.entry

sw.one:
  %3 = call i32 @fib(i32 6)
  br label %end

sw.two:
  %4 = mul i32 %2, %2
  call void @print(i32 %4)
  br label %loop.outer.entry

sw.default:
  %5 = add i32 %2, 10
  call void @print(i32 %5)
  br label %end

loop.outer.entry:
  br label %loop.outer

loop.outer:
  %i.out = phi i32 [ 0, %loop.outer.entry ], [ %i.next.out, %loop.inner.exit ]
  %cmp.out = icmp slt i32 %i.out, 3
  br i1 %cmp.out, label %loop.inner.entry, label %end

loop.inner.entry:
  br label %loop.inner

loop.inner:
  %i.in = phi i32 [ 0, %loop.inner.entry ], [ %i.next.in, %loop.inner ]
  %acc = phi i32 [ 0, %loop.inner.entry ], [ %acc.next, %loop.inner ]
  %mul = mul i32 %i.in, %i.out
  %acc.next = add i32 %acc, %mul
  %i.next.in = add i32 %i.in, 1
  %cmp.in = icmp slt i32 %i.next.in, 5
  br i1 %cmp.in, label %loop.inner, label %loop.inner.exit

loop.inner.exit:
  call void @print(i32 %acc.next)
  %i.next.out = add i32 %i.out, 1
  br label %loop.outer

end:
  %retv = phi i32 
      [ %3, %sw.one ], 
      [ %5, %sw.default ], 
      [ %2, %loop.outer ]
  ret i32 %retv
}

