target triple = "x86_64-pc-linux-gnu"
@pfmt = constant [5 x i8] c"%ld\0A\00"
declare i32 @printf(i8*,...)
@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.int = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
declare i32 @scanf(i8*,...)
define i32 @main() {

;Building NewStmt instruction:
    %v1 = alloca i64   ; ptr to read into      
    store i64 3, i64* %v1
    %v2 = load i64, i64* %v1

;Building write stmt
    %v3 = bitcast [5 x i8]* @pfmt to i8*
    call i32(i8*,...) @printf(i8* %v3, i64 %v2)
    %v4 = load i64, i64* %v1
    %v5 = add i64 %v4, 3

;Building write stmt
    %v6 = bitcast [5 x i8]* @pfmt to i8*
    call i32(i8*,...) @printf(i8* %v6, i64 %v5)
    %v7 = ptrtoint i64(i64)* @fun1 to i64
    ret i32 0
}
define i64 @fun1 (i64 %fun1var){

;Building write stmt
    %v8 = bitcast [5 x i8]* @pfmt to i8*
    call i32(i8*,...) @printf(i8* %v8, i64 2)
    ret i64 0
}
