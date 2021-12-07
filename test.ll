target triple = "x86_64-pc-linux-gnu"
@pfmt = constant [5 x i8] c"%ld\0A\00"
declare i32 @printf(i8*,...)
@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.int = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
declare i32 @scanf(i8*,...)
define i32 @main() {
    %v1 = ptrtoint i64(i64)* @fun1 to i64

;Building NewStmt instruction:
    %v2 = alloca i64   ; ptr to read into      
    store i64 %v1, i64* %v2
    %v3 = load i64, i64* %v2
    %v5 = inttoptr i64%v3 to i64 (i64)*
    %v4 =  call i64 %v5 ( i64 2333)    ret i32 0
}
define i64 @fun1 (i64 %fun1var){
    %v6 = load i64, i64* %fun1var

;Building write stmt
    %v7 = bitcast [5 x i8]* @pfmt to i8*
    call i32(i8*,...) @printf(i8* %v7, i64 %v6)
    ret i64 0
}
