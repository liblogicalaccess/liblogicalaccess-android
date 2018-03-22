package com.islog.liblogicalaccess;


import android.nfc.Tag;
import android.nfc.tech.IsoDep;
import android.nfc.tech.MifareClassic;
import android.nfc.tech.MifareUltralight;
import android.nfc.tech.NfcA;

import com.islog.rfidguard.Utils;
import com.orhanobut.logger.Logger;

import java.io.IOException;

/**
 * Created by Adrien on 23/01/14.
 */
@SuppressWarnings("HardCodedStringLiteral")
public class AndroidTag {
    private static final String Mifare1KType = "Mifare1K";

    private static final Object lock = new Object();
    private static NfcA mNfcA = null;
    private static MifareClassic mMifareClassic = null;
    private static IsoDep mIsoDep = null;
    private static Tag myTag = null;
    private static String myCardType = "";

    private AndroidTag() {
    }

    private static String getTagInfo(Tag tag) {
        String info = "";

        // Tech List
        String[] techList = tag.getTechList();

        for (String techno : techList) {
            // Mifare Classic/UltraLight Info
            String type = "Unknown";
            if (techno.equals(MifareClassic.class.getName())) {
                MifareClassic mifareClassicTag = MifareClassic.get(tag);

                // Type Info
                switch (mifareClassicTag.getType()) {
                    case MifareClassic.TYPE_CLASSIC:
                        switch (mifareClassicTag.getSize()) {
                            case MifareClassic.SIZE_MINI:
                            case MifareClassic.SIZE_1K:
                            case MifareClassic.SIZE_2K:
                                type = Mifare1KType;
                                break;
                            case MifareClassic.SIZE_4K:
                                type = "Mifare4K";
                                break;
                        }
                        break;
                    case MifareClassic.TYPE_PLUS:
                        type = "MifarePlus2K";
                        break;
                    default:
                        throw new RuntimeException("Unhandled Mifare Classic type");
                }
                info = type;
            }
            else if (techno.equals(MifareUltralight.class.getName())) {
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
                info = type;
            }
            else if (techno.equals(IsoDep.class.getName())) {
                info = "DESFire"; //We guess it...
            }/* else if(techList[i].equals(Ndef.class.getName())) {
                Ndef ndefTag = Ndef.get(tag);
                info += "Is Writable: " + ndefTag.isWritable() + "\n" +
                        "Can Make ReadOnly: " + ndefTag.canMakeReadOnly() + "\n";
            } else if(techList[i].equals(NdefFormatable.class.getName())) {
                NdefFormatable ndefFormatableTag = NdefFormatable.get(tag);
            }*/

            Logger.d("TechDetected: " + techno + " -> guess type: " + info);
        }
        return info;
    }

    public static void setCurrentCard(Tag tag) {
        synchronized (lock) {
            myTag = tag;

            mIsoDep = IsoDep.get(tag);
            mNfcA = NfcA.get(tag);
            mMifareClassic = MifareClassic.get(tag);

            myCardType = getTagInfo(tag);

            if (myCardType.equals("")) {
                if (mIsoDep != null) {
                    Logger.d("ISODEP id: " + Utils.bytesToHex(tag.getId()));
                    myCardType = "DESFire"; //lets say it is desfire...
                }
                else if (mMifareClassic != null) {
                    Logger.d("Mifare id: " + Utils.bytesToHex(tag.getId()));
                    myCardType = Mifare1KType; //Lets say it is 1K...
                }
                else if (mNfcA != null) {
                    Logger.d("NFCA id: " + Utils.bytesToHex(tag.getId()));
                    myCardType = Mifare1KType; //Lets say it is Mifare1k...
                }
            }

            Logger.d("setCurrentCard: " + myCardType);
        }
    }

    public static String getCurrentCardType() throws Exception {
        synchronized (lock) {
            Logger.d(myCardType);
            return myCardType;
        }
    }

    public static byte[] getUID() throws Exception {
        synchronized (lock) {
            if (myTag != null) {
                Logger.d("Card UID: %s", Utils.bytesToHex(myTag.getId()));
                return myTag.getId();
            }
            return null;
        }
    }

    public static boolean connect() throws Exception {
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
                }
                else if (mMifareClassic != null) {
                    if (!mMifareClassic.isConnected()) {
                        mMifareClassic.setTimeout(1000);
                        mMifareClassic.connect();
                        Logger.d("Mifare connect");
                    }
                }
                else if (mNfcA != null) {
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

    public static void disconnect() throws Exception {
        synchronized (lock) {
            if (myTag == null)
                return;
            try {
                if (mIsoDep != null) {
                    mIsoDep.close();
                    Logger.d("ISODep disconnect");
                }
                else if (mMifareClassic != null) {
                    mMifareClassic.close();
                    Logger.d("Mifare disconnect");
                }
                else if (mNfcA != null) {
                    mNfcA.close();
                    Logger.d("NFCA disconnect");
                }
                removeCard();
            } catch (IOException e) {
                Logger.e(e, "Disconnect failed");
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
                }
                else if (mNfcA != null) {
                    //Logger.d("NFCA transceive %s", bytesToHex(cmd));
                    ret = mNfcA.transceive(cmd);
                }
                //Logger.d("recieved %s", bytesToHex(cmd));
                return ret;
            } catch (IOException e) {
                Logger.e(e, "ExecCommand failed");
                removeCard();
                throw e;
            }
        }
    }

    public static byte[] mifareReadBlock(int sector, int block, byte[] key) throws Exception {
        synchronized (lock) {
            if (myTag == null)
                return null;
            try {
                byte[] ret = null;

                if (mMifareClassic == null) {
                    throw new Exception("The device does not support reading Mifare.");
                }

                if (!mMifareClassic.authenticateSectorWithKeyA(sector, key))
                    throw new Exception("Authentication failed to sector " + sector);

                int blockTorRead = mMifareClassic.sectorToBlock(sector) + block;

                ret = mMifareClassic.readBlock(blockTorRead);

                Logger.d("recieved %s", Utils.bytesToHex(ret));
                return ret;
            } catch (Exception e) {
                Logger.e(e, "MifareReadBlock failed");
                removeCard();
                throw e;
            }
        }
    }

    public static void removeCard() {
        mNfcA = null;
        mMifareClassic = null;
        mIsoDep = null;
        myTag = null;
        myCardType = "";
    }
}

