package com.wizd.xposedinlinehook;

import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import java.io.FileOutputStream;
import java.lang.reflect.Method;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedHelpers;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

public class HookClass implements IXposedHookLoadPackage {
    volatile Context g_appContext = null;
    public static final String dataPath = "/data/data/com.wizd.xposedinlinehook";

    static {
//        System.loadLibrary("native-lib");
        System.load(dataPath + "/lib/libnative-lib.so");
    }

    @Override
    public void handleLoadPackage(XC_LoadPackage.LoadPackageParam loadPackageParam) throws Throwable {
        if (!loadPackageParam.packageName.equals("com.yourpackage"))
            return;

        XposedHelpers.findAndHookMethod(Application.class, "attach", Context.class, new XC_MethodHook() {
            @Override
            protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                if(g_appContext == null){
                    g_appContext = (Context) param.args[0];
                    Context appContext = (Context) param.args[0];
                    realHook(appContext);
                }
            }
        });

    }

    private void realHook(final Context appContext) {
        hookLoginActivity(appContext);
    }

    private void hookLoginActivity(final Context appContext){
        XposedHelpers.findAndHookConstructor("com.yourpackage.YourActivity", appContext.getClassLoader(),
                new XC_MethodHook() {
                    @Override
                    protected void beforeHookedMethod(MethodHookParam param) throws Throwable {
                        super.beforeHookedMethod(param);

                        Log.i("wizd_xposed", "YourActivity.beforeHook.");

                        testNative();
                    }

                    @Override
                    protected void afterHookedMethod(MethodHookParam param) throws Throwable {
                        super.afterHookedMethod(param);
                    }
                }
        );
    }

    private void write2Disk(String str){
        try {
            FileOutputStream fout = new FileOutputStream("/sdcard/wizd_log.txt", true);
            byte[] bytes = str.getBytes();

            fout.write(bytes);
            fout.close();
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }

    private String printByteArray(byte[] arr){
        StringBuilder retStr = new StringBuilder();

        for (byte ele : arr){
            retStr.append(String.format("%02x ", ele));
        }

        return retStr.toString();
    }


    private String printShortArray(short[] arr){
        StringBuilder retStr = new StringBuilder();

        for (short ele : arr){
            retStr.append(String.format("%04x ", ele));
        }

        return retStr.toString();
    }

    public static native void testNative();
}
