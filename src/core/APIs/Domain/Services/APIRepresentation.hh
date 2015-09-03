<?hh
namespace APIs\Domain\Services;
use APIs\Domain\API;

interface APIRepresentation {
    public function getUrl(): string;
    public function getOriginal(): string;
    public function getAPI(): API;
}
