<?hh
namespace APIs\Infrastructure\Objects\Representations;
use APIs\Domain\Services\APIRepresentation;
use APIs\Domain\API;

final class ConcreteAPIRepresentation implements APIRepresentation {
    private $url;
    private $original;
    private $api;
    public function __construct(string $url, string $original, API $api) {
        $this->url = $url;
        $this->original = $original;
        $this->api = $api;
    }

    public function getUrl(): string {
        return $this->url;
    }

    public function getOriginal(): string {
        return $this->original;
    }

    public function getAPI(): API {
        return $this->api;
    }

}
