#ifdef MTK_DSPIRDBG
/*dsp logger*/
X_CON_SCE(CON_SCE_SS_FCORE_LOGGING, 0x8008580A, 0x80805001, 0x01005010, 0x1818500A, 0x20205003, 0x40405013)
#endif

#if 0
/* video telephonly */
X_CON_SCE(CON_SCE_VT, 0x8008580D, 0x80805001, 0x01005011, 0x18185004, 0x20205004, 0x40405017)

/* video record */
X_CON_SCE(CON_SCE_VIDEO_REC, 0x80085803, 0x80805001, 0x01005011, 0x18185004, 0x20205004, 0x40405021)

/* camera capture */
X_CON_SCE(CON_SCE_CAM_CAP, 0x80085800, 0x80805001, 0x01005011, 0x18185004, 0x20205004, 0x40405019)

/* camera preview */
X_CON_SCE(CON_SCE_CAM_PREVIEW, 0x80085800, 0x80805001, 0x01005011, 0x18185004, 0x20205004, 0x40405015)

/* video playback with TV-out (Play mode: LCM(Menu) / TV(Video, No HDMI)) */
X_CON_SCE(CON_SCE_PLAYBACK_WITH_TVOUT, 0x8008580C, 0x80805001, 0x01005011, 0x18185004, 0x20205004, 0x40405018)

/* video playback without TV-out */
X_CON_SCE(CON_SCE_PLAYBACK_WITHOUT_TVOUT, 0x8008580B, 0x80805001, 0x01005011, 0x18185004, 0x20205004, 0x40405017)

/* normal */
X_CON_SCE(CON_SCE_NORMAL, 0x80085809, 0x80805001, 0x01005011, 0x18185004, 0x20205004, 0x40405011)
#else

/* video telephonly */
X_CON_SCE(CON_SCE_VT, 0x8008580D, 0x80805001, 0x01005012, 0x18185004, 0x20205004, 0x40407018)

/* video record */
X_CON_SCE(CON_SCE_VIDEO_REC, 0x80085803, 0x80805001, 0x01005012, 0x18185004, 0x20205004, 0x40407025)

/* camera capture */
X_CON_SCE(CON_SCE_CAM_CAP, 0x80085800, 0x80805001, 0x01005012, 0x18185004, 0x20205004, 0x4040701C)

/* camera preview */
X_CON_SCE(CON_SCE_CAM_PREVIEW, 0x80085800, 0x80805001, 0x01005012, 0x18185004, 0x20205004, 0x40407018)

/* video playback with TV-out (Play mode: LCM(Menu) / TV(Video, No HDMI)) */
X_CON_SCE(CON_SCE_PLAYBACK_WITH_TVOUT, 0x8008580D, 0x80805001, 0x01005012, 0x18185004, 0x20205004, 0x4040701F)

/* video playback without TV-out */
X_CON_SCE(CON_SCE_PLAYBACK_WITHOUT_TVOUT, 0x8008580D, 0x80805001, 0x01005012, 0x18185004, 0x20205004, 0x4040701B)

/* normal */
X_CON_SCE(CON_SCE_NORMAL, 0x80085809, 0x80805001, 0x01005012, 0x18185004, 0x20205004, 0x40407017)

#endif
