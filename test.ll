target triple = "x86_64-pc-linux-gnu"
@pfmt = constant [5 x i8] c"%ld\0A\00"
declare i32 @printf(i8*,...)
declare i32 @rand()
declare double @sqrt(double)
@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.int = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
declare i32 @scanf(i8*,...)
define i32 @main() {
    %v2 = ptrtoint i64(i64)* @randBuiltIn to i64
    %v1 = alloca i64
    store i64 %v2, i64* %v1
    %v4 = ptrtoint i64(i64)* @sqrtBuiltIn to i64
    %v3 = alloca i64
    store i64 %v4, i64* %v3
    %v5 = load i64, i64* %v3
    %v7 = inttoptr i64 %v5 to i64 (i64)*
    %v6 =  call i64 %v7 ( i64 169)
;Building write stmt
    %v8 = bitcast [5 x i8]* @pfmt to i8*
    call i32(i8*,...) @printf(i8* %v8, i64 %v6)
    ret i32 0
}
define i64 @randBuiltIn (i64 %randarg){
    %v9 = call i32 @rand()
    %v10 = zext i32 %v9 to i64
    %v11 = srem i64 %v10, %randarg
    ret i64 %v11
}

define i64 @sqrtBuiltIn (i64 %sqrtarg){
    %v12 = sitofp i64 %sqrtarg to double
    %v13 = call double @sqrt(double %v12)
    %v14 = fptoui double %v13 to i64
    ret i64 %v14
}

