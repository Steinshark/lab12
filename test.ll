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
    call i32(i8*,...) @printf(i8* getelementptr inbounds ([25 x i8], [ 25 x i8]*@.2, i64 0, i64 0))
    %v3 = load i64, i64* %v2
    %v5 = inttoptr i64%v3 to i64 (i64)*
    %v4 =  call i64 %v5 ( i64 5)
;Building write stmt
    %v6 = bitcast [5 x i8]* @pfmt to i8*
    call i32(i8*,...) @printf(i8* %v6, i64 %v4)
    call i32(i8*,...) @printf(i8* getelementptr inbounds ([26 x i8], [ 26 x i8]*@.3, i64 0, i64 0))
    ret i32 0
}
define i64 @fun1 (i64 %fun1var){
    ;Binding fun argument
    %v7 = alloca i64
    store i64 %fun1var, i64* %v7
    ;Binding return value, init to 0
    %v8 = alloca i64    store i64 0, i64* %v8
    %v9 = load i64, i64* %v7

;Building write stmt
    %v10 = bitcast [5 x i8]* @pfmt to i8*
    call i32(i8*,...) @printf(i8* %v10, i64 %v9)
    %v11= load i64, i64* %v8
    ret i64  %v11
}
@.2 = private unnamed_addr constant [25x i8] c"just built the function\0A\00", align 1
@.3 = private unnamed_addr constant [26x i8] c"just called the function\0A\00", align 1
