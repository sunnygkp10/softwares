<?hh
namespace APIs\Domain\Services;
use APIs\Domain\Services\APIRepresentation;

interface APIService {
    public function save(APIRepresentation $representation): void;
}
