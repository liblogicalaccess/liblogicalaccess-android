package com.islog.liblogicalaccess;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.util.Log;
import android.os.Bundle;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.nfc.tech.IsoDep;
import android.nfc.tech.MifareClassic;
import android.nfc.tech.NfcA;
import android.widget.Toast;

import com.islog.liblogicalaccess.AndroidTag;

/**
 * Created by Adrien on 27/03/2015.
 */
public class NFCAndroid implements NfcAdapter.ReaderCallback {

    private static Activity mInstance = null;

    public NFCAndroid(Activity context) {

        mInstance = context;

        if (!mInstance.getPackageManager().hasSystemFeature(PackageManager.FEATURE_NFC)) {
            Toast.makeText(mInstance.getApplicationContext(), "Your device does not support NFC. You will not be able to encode cards.", Toast.LENGTH_LONG).show();
        }

        NfcAdapter adapter = NfcAdapter.getDefaultAdapter(context);

        if (adapter != null && adapter.isEnabled()) {
            Bundle options = new Bundle();
            options.putInt(NfcAdapter.EXTRA_READER_PRESENCE_CHECK_DELAY, 2); //should be 5000 but this tablet seems to us seconds ? wtf
            adapter.enableReaderMode(context, this, NfcAdapter.FLAG_READER_NFC_A | NfcAdapter.FLAG_READER_SKIP_NDEF_CHECK, options);
            Log.d("NFCAndroid", "Change check delay");
        }
    }

    public void onTagDiscovered(Tag tag) {
        AndroidTag.setCurrentCard(tag);
    }

    public static synchronized void openSettingWindow()
    {
        Log.d("NFCAndroid", "openSettingWindow");
        if (android.os.Build.VERSION.SDK_INT >= 16) {
            mInstance.startActivity(new Intent(android.provider.Settings.ACTION_NFC_SETTINGS));
        }
    }

    public static synchronized boolean checkNFCEnabled()
    {
        NfcAdapter adapter = NfcAdapter.getDefaultAdapter(mInstance);

        Log.d("NFCAndroid", "checkNFCEnabled");
        if (adapter != null && !adapter.isEnabled()) {
            return false;
        }
        return true;
    }
}

