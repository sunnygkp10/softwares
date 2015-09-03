<?hh
namespace APIs\Domain\Https\Repositories;
use APIs\Domain\Https\HttpQuery;

interface HttpQueryRepository {
    public function retrieve(Map<string, HttpQueryRetrieverCriteria> $criterias): Map<string, HttpQuery>;
}
