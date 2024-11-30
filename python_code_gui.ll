; ModuleID = 'PythonCompiler'
source_filename = "PythonCompiler"

define void @main() {
entry:
  %x = alloca i32, align 4
  store i32 2, ptr %x, align 4
  %y = alloca i32, align 4
  store i32 8, ptr %y, align 4
  %x1 = load i32, ptr %x, align 4
  %y2 = load i32, ptr %y, align 4
  %addtmp = add i32 %x1, %y2
  %z = alloca i32, align 4
  store i32 %addtmp, ptr %z, align 4
  %z3 = load i32, ptr %z, align 4
  call void @print(i32 %z3)
  %x4 = load i32, ptr %x, align 4
  %y5 = load i32, ptr %y, align 4
  %multmp = mul i32 %x4, %y5
  %z6 = alloca i32, align 4
  store i32 %multmp, ptr %z6, align 4
  %z7 = load i32, ptr %z6, align 4
  call void @print(i32 %z7)
  %y8 = load i32, ptr %y, align 4
  %x9 = load i32, ptr %x, align 4
  %divtmp = sdiv i32 %y8, %x9
  %z10 = alloca i32, align 4
  store i32 %divtmp, ptr %z10, align 4
  %z11 = load i32, ptr %z10, align 4
  call void @print(i32 %z11)
  %y12 = load i32, ptr %y, align 4
  %x13 = load i32, ptr %x, align 4
  %subtmp = sub i32 %y12, %x13
  %z14 = alloca i32, align 4
  store i32 %subtmp, ptr %z14, align 4
  %z15 = load i32, ptr %z14, align 4
  call void @print(i32 %z15)
  ret void
}

declare void @print(i32)
