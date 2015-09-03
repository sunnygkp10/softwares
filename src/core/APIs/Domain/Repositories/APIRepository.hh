<?hh
namespace APIs\Domain\Repositories;
use APIs\Domain\API;

interface APIRepository {
    public function retrieve(APIRetrieverCriteria $criteria): API;
}
