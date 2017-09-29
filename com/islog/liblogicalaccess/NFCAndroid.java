package com.islog.liblogicalaccess;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.util.Log;
import android.os.Bundle;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.widget.Toast;

import com.islog.cardanalyser.R;
import com.orhanobut.logger.Logger;

/**
 * Created by Adrien on 27/03/2015.
 */
public class NFCAndroid implements NfcAdapter.ReaderCallback {

    private Activity mInstance = null;
    private static NFCEvent mNfcEvent = null;

    public NFCAndroid(Activity context, NFCEvent nfcEvent) {

        mInstance = context;
        mNfcEvent = nfcEvent;

        if (!mInstance.getPackageManager().hasSystemFeature(PackageManager.FEATURE_NFC)) {
            Toast.makeText(mInstance, R.string.device_no_nfc, Toast.LENGTH_LONG).show();
            return;
        }

        enableNFCEvent();
    }

    public void enableNFCEvent() {
        NfcAdapter adapter = NfcAdapter.getDefaultAdapter(mInstance);

        if (adapter != null && adapter.isEnabled()) {
            Bundle options = new Bundle();
            options.putInt(NfcAdapter.EXTRA_READER_PRESENCE_CHECK_DELAY, 2); //should be 5000 but this tablet seems to us seconds ? wtf
            adapter.enableReaderMode(mInstance, this, NfcAdapter.FLAG_READER_NFC_A | NfcAdapter.FLAG_READER_SKIP_NDEF_CHECK, options);
            Logger.d("Enable NFC events"); //NON-NLS
        }
    }

    public void disableNFCEvent() {
        NfcAdapter adapter = NfcAdapter.getDefaultAdapter(mInstance);
        if (adapter != null && adapter.isEnabled()) {
            adapter.disableReaderMode(mInstance);
            Logger.d("Disable NFC events"); //NON-NLS
        }
    }

    public void onTagDiscovered(Tag tag) {
        AndroidTag.setCurrentCard(tag);
        if (mNfcEvent != null)
            mNfcEvent.onTagDiscovered(tag);
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

