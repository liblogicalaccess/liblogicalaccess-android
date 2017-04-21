package com.islog.liblogicalaccess;

import android.content.Context;
import android.content.pm.ApplicationInfo;

/**
 * Created by adrien on 12/01/17.
 */

public class NDKHelper {
    private static Context mContext;

    public static void setContext(Context context) {
        mContext = context;
    }

    public static String getNativeLibraryDirectory() {
        ApplicationInfo ai = mContext.getApplicationInfo();

        if ((ai.flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP) != 0
                || (ai.flags & ApplicationInfo.FLAG_SYSTEM) == 0)
        {
            return ai.nativeLibraryDir;
        }
        return "/system/lib/"; //NON-NLS
    }
}
