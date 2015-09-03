<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Https\URLs\URL;

final class ConcreteURL implements URL {

    private $url;
    public function __construct(string $url) {

        if (!filter_var($url, FILTER_VALIDATE_URL)) {
            throw new \Exception('The given url ('.$url.') is not valid.', 500);
        }

        $this->url = $url;
    }

    public function get(): string {
        return $this->url;
    }

}
