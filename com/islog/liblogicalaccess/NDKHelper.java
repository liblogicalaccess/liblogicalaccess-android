package com.islog.liblogicalaccess;

import android.content.Context;
import android.content.pm.ApplicationInfo;

/**
 * Created by adrien on 12/01/17.
 */

public class NDKHelper {

    private static String NativeLibraryDirectory;

    public static void cacheNativeLibraryDirectory(Context context) {
        ApplicationInfo ai = context.getApplicationInfo();

        if ((ai.flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP) != 0
                || (ai.flags & ApplicationInfo.FLAG_SYSTEM) == 0)
        {
            NativeLibraryDirectory = ai.nativeLibraryDir;
        }
        else
            NativeLibraryDirectory = "/system/lib/"; //NON-NLS
    }

    public static String getNativeLibraryDirectory() {
        return NativeLibraryDirectory;
    }
}
