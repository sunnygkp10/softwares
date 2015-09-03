<?hh
namespace APIs\Domain\Https\URLs;

interface URLAdapter {
    public function convertStringToURL(string $url): URL;
}
