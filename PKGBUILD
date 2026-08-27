# Maintainer: Arch Chad <archchad@btw.i.use.arch>
pkgname=fatfetch-git
pkgver=4.2.0
pkgrel=1
pkgdesc="A meme-loaded, satirical fastfetch/neofetch clone written in C++ for Arch Linux enthusiasts"
arch=('x86_64')
url="https://github.com/archchad/FATfetch"
license=('MIT')
depends=('glibc')
makedepends=('gcc' 'make')
provides=('fatfetch')
conflicts=('fatfetch')
source=("fatfetch::git+file://$(pwd)")
md5sums=('SKIP')

build() {
    cd "$srcdir/fatfetch"
    make fatfetch
}

package() {
    cd "$srcdir/fatfetch"
    make DESTDIR="$pkgdir" PREFIX="/usr" install
}
