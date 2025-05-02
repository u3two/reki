{
  inputs = {
      nixpkgs-unstable.url = "github:NixOS/nixpkgs/nixos-unstable";
  };
  outputs = { self, nixpkgs-unstable }: 
    let
      pkgs = nixpkgs-unstable.legacyPackages.${system};
      system = "x86_64-linux";
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [ 
          cmake
          pkg-config
        ];

        buildInputs = with pkgs; [
          sdl3
          sdl3-ttf
          libcap
        ];
      };
    };
}
