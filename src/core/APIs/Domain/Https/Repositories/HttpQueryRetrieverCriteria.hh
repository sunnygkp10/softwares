<?hh
namespace APIs\Domain\Https\Repositories;
use APIs\Domain\Https\URLs\URL;

interface HttpQueryRetrieverCriteria {
    public function getUrl(): URL;
    public function getHttpMethod(): string;
    public function hasPort(): boolean;
    public function getPort(): ?integer;
    public function hasData(): ?boolean;
    public function getData(): ?Map<string, mixed>;
}
