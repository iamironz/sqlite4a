-keepattributes Signature,Exceptions,InnerClasses,EnclosingMethod,*Annotation*
-keepparameternames

-keep public interface sqlite4a.** { *; }

-keep class sqlite4a.** {
    public static <fields>;
    public static <methods>;
    public <methods>;
    native <methods>;
}
