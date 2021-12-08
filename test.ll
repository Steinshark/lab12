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
    %v2 = ptrtoint i64(i64)* @fun1 to i64

;Building NewStmt instruction:
    %v3 = alloca i64   ; ptr to read into      
    store i64 %v2, i64* %v3
    %v4 = load i64, i64* %v3
    %v6 = inttoptr i64%v4 to i64 (i64)*
    %v5 =  call i64 %v6 ( i64 20)    ret i32 0
}
define i64 @fun1 (i64 %fun1var){
    %v7 = alloca i64
    store i64 %fun1var, i64* %v7
;BOUND FUN LOCAL g to %v7
    %v8 = load i64, i64* %v7
    %v9 = add i64 2, %v8

;Building Asn instruction:
    store i64 %v9, i64* %v7
    %v10 = load i64, i64* %v7

;Building write stmt
    %v11 = bitcast [5 x i8]* @pfmt to i8*
    call i32(i8*,...) @printf(i8* %v11, i64 %v10)
    ret i64 0
}
