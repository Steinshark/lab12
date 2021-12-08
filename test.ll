target triple = "x86_64-pc-linux-gnu"
@pfmt = constant [5 x i8] c"%ld\0A\00"
declare i32 @printf(i8*,...)
declare i32 @rand()
declare double @sqrt(double)
declare i64 @regUse()
@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.int = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@globalVar = global i64 0
declare i32 @scanf(i8*,...)
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

;Building NewStmt instruction:
    %v7 = alloca i64   ; ptr to read into      
    store i64 3, i64* %v7
    %v8 = load i64, i64* %v5
    %v10 = inttoptr i64 %v8 to i64 (i64)*
    %v9 =  call i64 %v10 ( i64 3)
;Building write stmt
    %v11 = bitcast [5 x i8]* @pfmt to i8*
    call i32(i8*,...) @printf(i8* %v11, i64 %v9)
    ret i32 0
}
define i64 @randBuiltIn (i64 %randarg){
    %v12 = call i32 @rand()
    %v13 = zext i32 %v12 to i64
    %v14 = srem i64 %v13, %randarg
    ret i64 %v14
}

define i64 @sqrtBuiltIn (i64 %sqrtarg){
    %v15 = sitofp i64 %sqrtarg to double
    %v16 = call double @sqrt(double %v15)
    %v17 = fptoui double %v16 to i64
    ret i64 %v17
}

define i64 @regUseBuiltIn (){
    store i64 3, i64* @globalVar 
    ret i64 0
}

