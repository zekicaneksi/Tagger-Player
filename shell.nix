{ pkgs ? import <nixpkgs> { config.allowUnfree = true; } }:
pkgs.mkShell {
  buildInputs = with pkgs; [
    wxGTK32
    gnumake
	gtk3
	sqlite
  ];
  shellHook = ''
     export XDG_DATA_DIRS=$GSETTINGS_SCHEMAS_PATH
  '';
}
