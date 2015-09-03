<?hh
namespace APIs\Infrastructure\Objects\RetrieverCriterias;
use APIs\Domain\Https\URLs\URL;
use APIs\Domain\Https\Repositories\HttpQueryRetrieverCriteria;

final class ConcreteHttpQueryRetrieverCriteria implements HttpQueryRetrieverCriteria {
    private $url;
    private $httpMethod;
    private $port;
    private $data;
    public function __construct(URL $url, string $httpMethod, integer $port = null, Map<string, mixed> $data = null) {
        $this->url = $url;
        $this->httpMethod = $httpMethod;
        $this->port = $port;
        $this->data = $data;
    }

    public function getUrl(): URL {
        return $this->url;
    }

    public function getHttpMethod(): string {
        return $this->httpMethod;
    }

    public function hasPort(): boolean {
        return !empty($this->port);
    }

    public function getPort(): ?integer {
        return $this->port;
    }

    public function hasData(): ?boolean {
        return !empty($this->data);
    }

    public function getData(): ?Map<string, mixed> {
        return $this->data;
    }

}
