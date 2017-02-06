package com.islog.liblogicalaccess;

import android.nfc.Tag;

/**
 * Created by adrien on 06/02/17.
 */

public interface NFCEvent {
    void onTagDiscovered(Tag tag);
}