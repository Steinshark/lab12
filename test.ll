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
    %v7 = load i64, i64* %v5
    %v9 = inttoptr i64 %v7 to i64 (i64)*
    %v8 =  call i64 %v9 ( i64 2)
;Building write stmt
    %v10 = bitcast [5 x i8]* @pfmt to i8*
    call i32(i8*,...) @printf(i8* %v10, i64 %v8)
    ret i32 0
}
define i64 @randBuiltIn (i64 %randarg){
    %v11 = call i32 @rand()
    %v12 = zext i32 %v11 to i64
    %v13 = srem i64 %v12, %randarg
    ret i64 %v13
}

define i64 @sqrtBuiltIn (i64 %sqrtarg){
    %v14 = sitofp i64 %sqrtarg to double
    %v15 = call double @sqrt(double %v14)
    %v16 = fptoui double %v15 to i64
    ret i64 %v16
}

define i64 @regUseBuiltIn (){
    ret i64 16
}

