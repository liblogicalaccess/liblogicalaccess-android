package com.islog.liblogicalaccess;


import android.nfc.Tag;
import android.nfc.tech.IsoDep;
import android.nfc.tech.MifareClassic;
import android.nfc.tech.MifareUltralight;
import android.nfc.tech.NfcA;

import com.orhanobut.logger.Logger;

import java.io.IOException;

/**
 * Created by Adrien on 23/01/14.
 */
@SuppressWarnings("HardCodedStringLiteral")
public class AndroidTag {

    private static NfcA mNfcA = null;
    private static IsoDep mIsoDep = null;
    private static Tag myTag = null;
    private static String mycardType = "";
    private static final Object lock = new Object();

    private final static char[] hexArray = "0123456789ABCDEF".toCharArray();
    private static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for ( int j = 0; j < bytes.length; j++ ) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }

    private static String getTagInfo(Tag tag) {
        String info = "";

        // Tech List
        String[] techList = tag.getTechList();

        for (String aTechList : techList) {
            // Mifare Classic/UltraLight Info
            String type = "Unknown";
            if (aTechList.equals(MifareClassic.class.getName())) {
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

            } else if (aTechList.equals(MifareUltralight.class.getName())) {
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
            } else if (aTechList.equals(IsoDep.class.getName())) {
                info = "DESFire"; //We guess it...
            }/* else if(techList[i].equals(Ndef.class.getName())) {
                Ndef ndefTag = Ndef.get(tag);
                info += "Is Writable: " + ndefTag.isWritable() + "\n" +
                        "Can Make ReadOnly: " + ndefTag.canMakeReadOnly() + "\n";
            } else if(techList[i].equals(NdefFormatable.class.getName())) {
                NdefFormatable ndefFormatableTag = NdefFormatable.get(tag);
            }*/

            Logger.d("TechDetected: " + aTechList + " -> guess type: " + info);
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

            if (mycardType.equals("")) {
                if (mIsoDep != null) {
                    Logger.d("ISODEP id: " + bytesToHex(tag.getId()));
                    mycardType = "DESFire"; //lets say it is desfire...
                } else if (mNfcA != null) {
                    Logger.d("NFCA id: " + bytesToHex(tag.getId()));
                    mycardType = "Mifare1K"; //Lets say it is Mifare1k...
                }
            }

            Logger.d("setCurrentCard: " + mycardType);
        }
    }

    public static String getCurrentCardType() throws Exception
    {
        synchronized (lock) {
            Logger.d(mycardType);
            return mycardType;
        }
    }

    public static byte[] getUID() throws Exception
    {
        synchronized (lock) {
            if (myTag != null) {
                Logger.d("Card UID: %s", bytesToHex(myTag.getId()));
                return myTag.getId();
            }
            return null;
        }
    }

    public static boolean connect() throws Exception
    {
        synchronized (lock) {
            if (myTag == null)
                return false;
            try {
                if (mIsoDep != null) {
                    if (!mIsoDep.isConnected()) {
                        mIsoDep.setTimeout(1000);
                        mIsoDep.connect();
                        Logger.d("ISODep connect");
                    }
                } else if (mNfcA != null) {
                    if (!mNfcA.isConnected()) {
                        mNfcA.setTimeout(1000);
                        mNfcA.connect();
                        Logger.d("NFCA connect");
                    }
                }
            } catch (IOException e) {
                Logger.e(e, "Connection failed");
                removeCard();
                throw e;
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
                    Logger.d("ISODep disconnect");
                } else if (mNfcA != null) {
                    mNfcA.close();
                    Logger.d("NFCA disconnect");
                }
                removeCard();
            } catch (IOException e) {
                Logger.e(e, "Connection failed");
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
                byte[] ret = null;
                if (mIsoDep != null) {
                    //Logger.d("ISODep transceive %s", bytesToHex(cmd));
                    ret = mIsoDep.transceive(cmd);
                } else if (mNfcA != null) {
                    //Logger.d("NFCA transceive %s", bytesToHex(cmd));
                    ret = mNfcA.transceive(cmd);
                }
                //Logger.d("recieved %s", bytesToHex(cmd));
                return ret;
            } catch (IOException e) {
                Logger.e(e, "Connection failed");
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

