<?php

class DB
{
    private $host;
    private $username;
    private $password;
    private $database;
    private $conn;

    public function __construct($host, $username, $password, $database)
    {
        $this->host = $host;
        $this->username = $username;
        $this->password = $password;
        $this->database = $database;
    }

    public function connect(): bool
    {
        try {
            $this->conn = new PDO(
                "mysql:host=$this->host;dbname=$this->database",
                $this->username,
                $this->password
            );
            $this->conn->setAttribute(
                PDO::ATTR_ERRMODE,
                PDO::ERRMODE_EXCEPTION
            );
            return true;
        } catch (PDOException $e) {
            echo "Connection failed: ".$e->getMessage();
            return false;
        }
    }

    public function updateSolutionStatus($id, $newStatus): void
    {
        try {
            $sql = "UPDATE solutions SET status=:newStatus WHERE id=:id";
            $stmt = $this->conn->prepare($sql);
            $stmt->bindParam(':newStatus', $newStatus);
            $stmt->bindParam(':id', $id);
            $stmt->execute();
            echo "Status updated successfully";
        } catch (PDOException $e) {
            echo "Error updating status: ".$e->getMessage();
        }
    }

    public function getSolutionStatus($id)
    {
        $stmt = $this->conn->prepare(
            "SELECT status FROM solutions WHERE id = :id"
        );
        $stmt->bindParam(':id', $id);
        $stmt->execute();
        $status = $stmt->fetchColumn();
        return $status;
    }

    public function getIdsByStatus($status)
    {
        try {
            $sql = "SELECT id FROM solutions WHERE status = :status";
            $stmt = $this->conn->prepare($sql);
            $stmt->bindParam(':status', $status);
            $stmt->execute();
            $result = $stmt->fetchAll(PDO::FETCH_ASSOC);
            $ids = [];
            foreach ($result as $row) {
                $ids[] = $row['id'];
            }
            return $ids;
        } catch (PDOException $e) {
            echo "Error retrieving ids: ".$e->getMessage();
            return [];
        }
    }

    public function close()
    {
        $this->conn = null;
    }
}
