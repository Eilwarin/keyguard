 #PKGBUILD

 pkgname=passkeep
 pkgver=v0.1
 pkgrel=1
 pkgdesc="A simple C++ password manager."
 arch=('x86_64')
 url="https://gitlab.com/use-cases1/passkeep"
 license=('GPL')
 depends=('qt5-base')

source=("${pkgname}-${pkgver}.tar.gz::https://gitlab.com/use-cases1/passkeep/-/archive/v0.1/${pkgname}-v0.1.tar.gz")

package() {
  cd "${srcdir}/${pkgname}-${pkgver}"
  ./cmake-build-debug/PassKeep
}
sha256sums=('b238a134ab4de6e31a204e1b688c4c353a18e237511fedc495ff83f3c0946053')
