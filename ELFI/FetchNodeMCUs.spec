# -*- mode: python ; coding: utf-8 -*-

block_cipher = None


a = Analysis(['FetchNodeMCUs.py'],
             pathex=['H:\\_ITI_Final\\MY_Repos\\GP_FOTA\\ELFI'],
             binaries=[],
             datas=[],
             hiddenimports=[],
             hookspath=['H:\\_ITI_Final\\MY_Repos\\GP_FOTA\\ELFI'],
             runtime_hooks=[],
             excludes=[],
             win_no_prefer_redirects=False,
             win_private_assemblies=False,
             cipher=block_cipher,
             noarchive=False)
pyz = PYZ(a.pure, a.zipped_data,
             cipher=block_cipher)
exe = EXE(pyz,
          a.scripts,
          a.binaries,
          a.zipfiles,
          a.datas,
          [],
          name='FetchNodeMCUs',
          debug=False,
          bootloader_ignore_signals=False,
          strip=False,
          upx=True,
          upx_exclude=[],
          runtime_tmpdir=None,
          console=False )
