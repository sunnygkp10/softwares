<?hh
namespace APIs\Domain\Repositories;

interface APIRetrieverCriteria {
    public function hasName(): boolean;
    public function getName(): string;
}
