declare void @print(i32)
declare i32 @foo(i32)

define i32 @test(i32 %x) {
entry:
  %cond = icmp slt i32 %x, 5
  br i1 %cond, label %then, label %else

then:
  %y = call i32 @foo(i32 %x)
  br label %merge

else:
  %z = add i32 %x, 10
  call void @print(i32 %z)
  br label %merge

merge:
  %m = phi i32 [ %y, %then ], [ %z, %else ]
  %cond2 = icmp sgt i32 %m, 3
  br i1 %cond2, label %loop.entry, label %ret.block

loop.entry:
  br label %0

0:
  %i = phi i32 [ 0, %loop.entry ], [ %i.next, %1 ]
  %sum = phi i32 [ 0, %loop.entry ], [ %sum.next, %1 ]
  %cmp = icmp slt i32 %i, 4
  br i1 %cmp, label %1, label %loop.exit

1:
  %tmp = add i32 %sum, %i
  %i.next = add i32 %i, 1
  %sum.next = add i32 %tmp, %m
  br label %0

loop.exit:
  call void @print(i32 %sum)
  br label %ret.block

ret.block:
  %retv = phi i32 [ %m, %merge ], [ %sum, %loop.exit ]
  ret i32 %retv
}

