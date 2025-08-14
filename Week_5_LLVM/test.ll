define void @foo() {
entry:
  br label %loop

loop:
  %i = phi i32 [ 0, %entry ], [ %inc, %loop ]
  %cond = icmp slt i32 %i, 5
  br i1 %cond, label %body, label %exit

body:
  %inc = add i32 %i, 1
  br label %loop

exit:
  ret void
}

