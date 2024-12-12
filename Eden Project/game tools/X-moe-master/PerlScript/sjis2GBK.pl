use Encode;
use Encode::CN;
use Win32::File;
my @files = <*.txt>;
mkdir 'scr_GBK';
foreach my $file (@files) {
    my $attr;
    Win32::File::GetAttributes($file, $attr);
    my $newfile = "scr_GBK\\$file";
    open OLD, $file or die "open $file failed: $!";
    open NEW, ">$newfile" or die "open $newfile failed: $!";
    while (my $line = <OLD>) {
        $line = decode("Shift-JIS", $line);
        print NEW encode("GBK", $line);
    }
    close OLD;
    close NEW;
    print "$file change to GBK\n";
}