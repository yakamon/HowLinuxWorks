package main

import (
	"errors"
	"fmt"
	"os"
	"strconv"
)

// Tower is hanoi tower.
type Tower struct {
	disks *Stack
}

// NewTower returns new hanoi tower.
func NewTower() *Tower {
	return &Tower{NewStack()}
}

// Add adds disk to Tower.
func (t *Tower) Add(disk int) error {
	if t.Height() != 0 && t.Top() < disk {
		return fmt.Errorf("Cannot add dist that has larger diameter than top disk: %v", disk)
	}
	t.disks.Push(disk)
	return nil
}

// Pop removes top disk from Tower.
func (t *Tower) Pop() int {
	return t.disks.Pop()
}

// Top returns top disk of Tower.
func (t *Tower) Top() int {
	n, err := t.disks.Top()
	if err != nil {
		panic(err)
	}
	return n
}

// Height returns height of Tower.
func (t *Tower) Height() int {
	return t.disks.Len()
}

// MoveTopTo moves Tower's top disk to dest Tower.
func (t *Tower) MoveTopTo(dest *Tower) {
	dest.Add(t.Pop())
}

// MoveDisks moves disks from t to dest using buf
func (t *Tower) MoveDisks(n int, dest, buf *Tower) {
	if n == 0 {
		return
	}
	t.MoveDisks(n-1, buf, dest)
	fmt.Println(t.disks, buf.disks, dest.disks)
	t.MoveTopTo(dest)
	fmt.Println(t.disks, buf.disks, dest.disks)
	buf.MoveDisks(n-1, dest, t)
}

// Stack is stack data structure for hanoi.
type Stack struct {
	data []int
}

// NewStack returns new stack.
func NewStack() *Stack {
	return &Stack{[]int{}}
}

// Push pushes value to Stack.
func (s *Stack) Push(n int) {
	s.data = append(s.data, n)
}

// Pop removes value from Stack and return it.
func (s *Stack) Pop() int {
	n, err := s.Top()
	if err != nil {
		panic(err)
	}
	s.data = s.data[:s.Len()-1]
	return n
}

// Top returns top element of Stack.
func (s *Stack) Top() (int, error) {
	if s.Len() == 0 {
		return -1, errors.New("Stack is empty")
	}
	return s.data[s.Len()-1], nil
}

// Len returns the length of Stack.
func (s *Stack) Len() int {
	return len(s.data)
}

func main() {
	ndisks, _ := strconv.Atoi(os.Args[1])

	origin, buf, dest := NewTower(), NewTower(), NewTower()
	for i := ndisks - 1; i >= 0; i-- {
		origin.Add(2*i + 3)
	}

	fmt.Println(origin.disks, buf.disks, dest.disks)

	origin.MoveDisks(ndisks, dest, buf)

	fmt.Println(origin.disks, buf.disks, dest.disks)
}
