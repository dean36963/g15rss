#Maintainer: Dean Birch <dean.birch0@gmail.com>
pkgname=g15rss-svn 
pkgver=0.2
pkgrel=1
pkgdesc="Displays RSS feeds on the LCD screen of a G15 keyboard (or similar)."
arch=('i686') #Probably all - but not tested
url=('https://github.com/dean36963/g15rss')
license=('Unknown')
depends=('g15daemon-svn' 'libg15render-svn' 'libg15-svn')
#or try below if you havent got svn version!
#depends=('g15daemon' 'libg15render' 'libg15')
source=(https://github.com/downloads/dean36963/g15rss/g15rss-svn-0.2-1.tar.gz)
md5sums=('eb48af541b0d1df6f01de4d4373b9bb3')

build() {
 cd "$srcdir"
 make
}

package() {
 mkdir -p "$pkgdir/usr/bin/"
 cp "$srcdir/g15rss" "$pkgdir/usr/bin/"
}

