; ModuleID = 'PythonCompiler'
source_filename = "PythonCompiler"

define void @main() {
entry:
  %x = alloca i32, align 4
  store i32 5, ptr %x, align 4
  %y = alloca i32, align 4
  store i32 10, ptr %y, align 4
  %x1 = load i32, ptr %x, align 4
  %y2 = load i32, ptr %y, align 4
  %addtmp = add i32 %x1, %y2
  %z = alloca i32, align 4
  store i32 %addtmp, ptr %z, align 4
  %z3 = load i32, ptr %z, align 4
  call void @print(i32 %z3)
  ret void
}

declare void @print(i32)
