 #PKGBUILD

 pkgname=passkeep
 pkgver=main
 pkgrel=1
 pkgdesc="A simple C++ password manager."
 arch=('x86_64')
 url="https://gitlab.com/use-cases1/passkeep"
 license=('GPL')
 depends=('qt5-base')

source=("passkeep-main.tar.gz::https://gitlab.com/use-cases1/passkeep/-/archive/main/passkeep-main.tar.gz")

package() {
  cd "${pkgname}-${pkgver}"
  make INSTALL_ROOT="${pkgdir}" install
}
