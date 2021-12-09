target triple = "x86_64-pc-linux-gnu"
@pfmt = constant [5 x i8] c"%ld\0A\00"
declare i32 @printf(i8*,...)
declare i32 @rand()
declare double @sqrt(double)
declare i64 @regUse()
@longFormatString = private unnamed_addr constant [4 x i8] c"%ld\00", align 1
@.int = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@regCounterPtr = global i64 0
@globalVar = global i64 0
declare i64 @scanf(i8*,...)
define i32 @main() {
    %regCounterPtr = alloca i64
    %v2 = ptrtoint i64(i64)* @randBuiltIn to i64
    %v1 = alloca i64
    store i64 %v2, i64* %v1
    %v4 = ptrtoint i64(i64)* @sqrtBuiltIn to i64
    %v3 = alloca i64
    store i64 %v4, i64* %v3
    %v6 = ptrtoint i64()* @regUseBuiltIn to i64
    %v5 = alloca i64
    store i64 %v6, i64* %v5

;Building CompOp instruction:
    %v7 = icmp slt i64 10 , 2
    %v8 = zext i1 %v7 to i64      ; must cast from i1 to i64 
    br label %b1   ;I was added in an IFELSE

;Building IfStmt
b1:
    %v9 = trunc i64 %v8 to i1
    br i1 %v9, label %b2, label %b3

;---TRUE
b2:
    %v10 = ptrtoint i64(i64)* @fun1 to i64

;Building write stmt
    %v11 = bitcast [5 x i8]* @pfmt to i8*
    call i32(i8*,...) @printf(i8* %v11, i64 %v10)
    br label %b4

;---FALSE
b3:

;Building write stmt
    %v12 = bitcast [5 x i8]* @pfmt to i8*
    call i32(i8*,...) @printf(i8* %v12, i64 1)
    br label %b4

;---END
b4:
    ret i32 0
}
define i64 @fun1 (i64 %fun1var){
;0x55d425124b40
;rand stored in %v1
%v14 = alloca i64
;regUse stored in %v5
%v15 = alloca i64
;sqrt stored in %v3
%v16 = alloca i64
    ;Binding fun argument
    %v13 = alloca i64
    store i64 %fun1var, i64* %v13
    ;Binding return value, init to 0
    %v17 = alloca i64    store i64 0, i64* %v17
    %v18 = load i64, i64* %v13

;Building write stmt
    %v19 = bitcast [5 x i8]* @pfmt to i8*
    call i32(i8*,...) @printf(i8* %v19, i64 %v18)
    %v20= load i64, i64* %v17
    ret i64  %v20
}
define i64 @randBuiltIn (i64 %randarg){
    %v21 = call i32 @rand()
    %v22 = zext i32 %v21 to i64
    %v23 = srem i64 %v22, %randarg
    ret i64 %v23
}

define i64 @sqrtBuiltIn (i64 %sqrtarg){
    %v24 = sitofp i64 %sqrtarg to double
    %v25 = call double @sqrt(double %v24)
    %v26 = fptoui double %v25 to i64
    ret i64 %v26
}

define i64 @regUseBuiltIn (){
    ret i64 26
}

