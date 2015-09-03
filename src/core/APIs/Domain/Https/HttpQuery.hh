<?hh
namespace APIs\Domain\Https;
use APIs\Domain\Https\URLs\URL;

interface HttpQuery {
    public function getUrl(): URL;
    public function getContent(): string;
    public function getHttpCode(): integer;
}
