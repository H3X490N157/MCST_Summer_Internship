define i32 @test_gcp() {
entry:
  ; вычисляем константы
  %a = add i32 5, 7       
  %b = mul i32 %a, 2      
  %c = sub i32 %b, 4      
  ret i32 %c
}
