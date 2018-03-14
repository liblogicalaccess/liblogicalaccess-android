package com.islog.liblogicalaccess;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.nfc.NfcAdapter;
import android.widget.Toast;

import com.islog.rfidguard.R;
import com.orhanobut.logger.Logger;

/**
 * Created by Adrien on 27/03/2015.
 */
public class NFCHelper {
    private Activity mInstance = null;

    public NFCHelper(Activity context, NFCEvent nfcEvent) {
        mInstance = context;

        if (!mInstance.getPackageManager().hasSystemFeature(PackageManager.FEATURE_NFC)) {
            Toast.makeText(mInstance, R.string.device_no_nfc, Toast.LENGTH_LONG).show();
            return;
        }
    }

    public synchronized void openSettingWindow() {
        Logger.d("Start NFC settings"); //NON-NLS
        mInstance.startActivityForResult(new Intent(android.provider.Settings.ACTION_NFC_SETTINGS), 1);
    }

    public synchronized boolean checkNFCEnabled()
    {
        NfcAdapter adapter = NfcAdapter.getDefaultAdapter(mInstance);

        if (adapter != null && !adapter.isEnabled()) {
            Logger.d("NFC is not enabled"); //NON-NLS
            return false;
        }
        Logger.d("NFC is enabled"); //NON-NLS
        return true;
    }
}
