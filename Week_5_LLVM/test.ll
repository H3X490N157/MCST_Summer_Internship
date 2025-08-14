define i32 @foo(i32 %0) {
entry:
  %1 = add i32 %0, 1
  ret i32 %1
}

define i32 @bar(i32 %0, i32 %1) {
entry:
  %2 = add i32 %0, %1
  %3 = mul i32 %2, 2
  ret i32 %3
}

define i32 @main() {
entry:
  %0 = call i32 @foo(i32 10)
  %1 = call i32 @bar(i32 3, i32 4)
  %2 = add i32 %0, %1
  ret i32 %2
}
