package com.islog.liblogicalaccess;


import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.nfc.tech.IsoDep;
import android.nfc.tech.MifareClassic;
import android.nfc.tech.MifareUltralight;
import android.nfc.tech.Ndef;
import android.nfc.tech.NdefFormatable;
import android.nfc.tech.NfcA;
import android.util.Log;
import java.io.ByteArrayOutputStream;
import java.io.IOException;

/**
 * Created by Adrien on 23/01/14.
 */
public class AndroidTag {

    private static NfcA mNfcA = null;
    private static IsoDep mIsoDep = null;
    private static Tag myTag = null;
    private static String mycardType = "";
    private static final Object lock = new Object();

    final protected static char[] hexArray = "0123456789ABCDEF".toCharArray();
    public static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for ( int j = 0; j < bytes.length; j++ ) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }

    private static String getTagInfo(Tag tag) {
        String info = new String();

        // Tech List
        String[] techList = tag.getTechList();

        // Mifare Classic/UltraLight Info
        String type = "Unknown";
        for(int i = 0; i < techList.length; i++) {
            if(techList[i].equals(MifareClassic.class.getName())) {
                MifareClassic mifareClassicTag = MifareClassic.get(tag);

                // Type Info
                switch (mifareClassicTag.getType()) {
                    case MifareClassic.TYPE_CLASSIC:
                        type = "Mifare1K";
                        break;
                    case MifareClassic.TYPE_PLUS:
                        type = "MifarePlus2K";
                        break;
                    /*case MifareClassic.TYPE_PRO:
                        type = "Pro";
                        break;*/
                }

            } else if(techList[i].equals(MifareUltralight.class.getName())) {
                MifareUltralight mifareUlTag = MifareUltralight.get(tag);

                // Type Info
                switch (mifareUlTag.getType()) {
                    case MifareUltralight.TYPE_ULTRALIGHT:
                        type = "MifareUltralight";
                        break;
                    case MifareUltralight.TYPE_ULTRALIGHT_C:
                        type = "MifareUltralightC";
                        break;
                }
                info = "Mifare " + type + "\n";
            } else if(techList[i].equals(IsoDep.class.getName())) {
                info = "DESFire"; //We guess it...
            }/* else if(techList[i].equals(Ndef.class.getName())) {
                Ndef ndefTag = Ndef.get(tag);
                info += "Is Writable: " + ndefTag.isWritable() + "\n" +
                        "Can Make ReadOnly: " + ndefTag.canMakeReadOnly() + "\n";
            } else if(techList[i].equals(NdefFormatable.class.getName())) {
                NdefFormatable ndefFormatableTag = NdefFormatable.get(tag);
            }*/

            Log.d("card detection", info);
        }
        return info;
    }

    public static void setCurrentCard(Tag tag)
    {
        synchronized (lock) {
            myTag = tag;

            mIsoDep = IsoDep.get(tag);
            mNfcA = NfcA.get(tag);

            mycardType = getTagInfo(tag);

            if (mycardType == "") {
                if (mIsoDep != null) {
                    Log.d("NFCAndroid", "ISODEP id: " + bytesToHex(tag.getId()));
                    mycardType = "DESFire"; //lets say it is desfire...
                } else if (mNfcA != null) {
                    Log.d("NFCAndroid", "NFCA id: " + bytesToHex(tag.getId()));
                    mycardType = "Mifare1K"; //Lets say it is Mifare1k...
                }
            }
        }
    }

    public static String getCurrentCardType() throws Exception
    {
        synchronized (lock) {
            return mycardType;
        }
    }

    public static byte[] getUID() throws Exception
    {
        synchronized (lock) {
            if (myTag != null)
                return myTag.getId();
            return null;
        }
    }

    public static boolean connect()
    {
        synchronized (lock) {
            if (myTag == null)
                return false;
            try {
                if (mIsoDep != null) {
                    if (!mIsoDep.isConnected()) {
                        mIsoDep.setTimeout(1000);
                        mIsoDep.connect();
                    }
                } else if (mNfcA != null) {
                    if (!mNfcA.isConnected()) {
                        mNfcA.setTimeout(1000);
                        mNfcA.connect();
                    }
                }
            } catch (IOException e) {
                removeCard();
                return false;
            }
            return true;
        }
    }

    public static void disconnect() throws Exception
    {
        synchronized (lock) {
            if (myTag == null)
                return;
            try {
                if (mIsoDep != null) {
                    mIsoDep.close();
                } else if (mNfcA != null) {
                    mNfcA.close();
                }
                removeCard();
            } catch (IOException e) {
                removeCard();
                throw e;
            }
        }
    }

    public static byte[] execCommand(byte[] cmd) throws IOException {
        synchronized (lock) {
            if (myTag == null)
                return null;
            try {
                if (mIsoDep != null) {
                    return mIsoDep.transceive(cmd);
                } else if (mNfcA != null) {
                    return mNfcA.transceive(cmd);
                }
                return null;
            } catch (IOException e) {
                removeCard();
                throw e;
            }
        }
    }

    public static void removeCard()
    {
        mNfcA = null;
        mIsoDep = null;
        myTag = null;
        mycardType = "";
    }
}

